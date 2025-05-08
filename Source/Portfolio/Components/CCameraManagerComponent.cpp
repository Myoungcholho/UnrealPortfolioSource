#include "../Components/CCameraManagerComponent.h"

#include "CWeaponComponent.h"
#include "../Global.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Portfolio/UnitTest/CCameraMoveTest.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Portfolio/Assassination/CCameraBlendAsset.h"
#include "Math/PerspectiveMatrix.h"

UCCameraManagerComponent::UCCameraManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FHelpers::GetAsset<UMaterialInstance>(&TransparentMaterial,"/Script/Engine.MaterialInstanceConstant'/Game/Material/M_Transparents_Inst.M_Transparents_Inst'");
}

void UCCameraManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	Camera = FHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	SpringArm = FHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	SpringArm->ProbeChannel = ECC_GameTraceChannel1;
	
	SubCameraActor = GetWorld()->SpawnActor<AActor>(SubCameraClass);
	if (SubCameraActor)
	{
		SubCameraActor->SetActorHiddenInGame(true);  // 기본적으로 숨김
	}

	OriginalArmLength = SpringArm->TargetArmLength;
}

void UCCameraManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleCameraCollision();

	if (!SpringArm) return;
	float targetLength = bZoomingOut ? ZoomTargetLength : OriginalArmLength;
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, targetLength, DeltaTime, ZoomInterpSpeed);
}

// 오브젝트 투명화 기능
void UCCameraManagerComponent::HandleCameraCollision()
{
	CheckNull(Camera);
	CheckNull(SpringArm);

	if(BoxHalfSize.IsNearlyZero())
		return;
	
	FVector CameraLocation = Camera->GetComponentLocation();
	FVector PlayerLocation = GetOwner()->GetActorLocation();
	FRotator BoxRotation = SpringArm->GetComponentRotation();
	
	// 현재 프레임에서 감지된 오브젝트
	TSet<AActor*> CurrentlyTransparentActors;
	
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(GetOwner());
	if(!!weapon)
	{
		AActor* weaponActor = Cast<AActor>(weapon->GetAttachment());
		if(!!weaponActor)
			ActorsToIgnore.Add(weaponActor);	// 본인 장착 무기 오브젝트
	}

	// 충돌할 오브젝트 타입 설정
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	/*ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));  // 정적 월드 오브젝트
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic)); // 동적 월드 오브젝트*/
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
	
	
	bool bHit = UKismetSystemLibrary::BoxTraceMultiForObjects
	(
		GetWorld(),
		CameraLocation,
		PlayerLocation,
		BoxHalfSize,
		BoxRotation,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebug,
		HitResults,
		true
	);

	// 1. 카메라 위치/방향 가져오기
	FVector Forward = Camera->GetForwardVector();
	FVector Right = Camera->GetRightVector();
	FVector Up = Camera->GetUpVector();

	// 2. 거리, FOV 설정
	float Near = GNearClippingPlane;
	float Far = FVector::Dist(CameraLocation, OwnerCharacter->GetActorLocation());
	float HalfFOV = FMath::DegreesToRadians(Camera->FieldOfView) * 0.5f;
	float AspectRatio = Camera->AspectRatio;

	// 3. 절두체 꼭짓점 생성
	FVector Corners[8];
	FHelpers::GenerateFrustumCorners(CameraLocation, Forward, Right, Up, Near, Far, HalfFOV, AspectRatio, Corners);

	// 4. 디버그로 절두체 그리기
	FHelpers::DrawFrustumFromCorners(GetWorld(), Corners, FColor::Green, 0.01f);

	// 5. 얻은 Actor 절두체에 들어가는지 검사
	if (bHit)
	{
		for (FHitResult& hit : HitResults)
		{
			AActor* HitActor = hit.GetActor();
			if (HitActor == nullptr) continue;

			FTransform TargetTransform = HitActor->GetActorTransform();
			UPrimitiveComponent* CollisionComponent = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
			if (CollisionComponent == nullptr) continue;

			FVector BoxExtent = FVector::ZeroVector;

			if (const UBoxComponent* Box = Cast<UBoxComponent>(CollisionComponent))
				BoxExtent = Box->GetScaledBoxExtent();
			else if (const UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(CollisionComponent))
			{
				float Radius = Capsule->GetScaledCapsuleRadius();
				float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
				BoxExtent = FVector(Radius, Radius, HalfHeight);
			}
			else if (const UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(CollisionComponent))
				BoxExtent = StaticMesh->Bounds.BoxExtent;
			else
				continue;
			
			FPlane FrustumPlanes[6]; // 미리 만든 절두체 평면 6개

			// 6. 절두체 내부에 한점이라도 포함된다면 투명화
			bool bInsideFrustum = FHelpers::IsAABBIntersectingFrustum(TargetTransform, BoxExtent, FrustumPlanes);
			if(bInsideFrustum == true)
			{
				MakeActorTransparent(HitActor);
				CurrentlyTransparentActors.Add(HitActor);
			}
		}
	}
	
	// 오브젝트 복구
	for (AActor* PreviouslyTransparentActor : PreviouslyTransparentActors)
	{
		if (!CurrentlyTransparentActors.Contains(PreviouslyTransparentActor))
		{
			ResetActorMaterial(PreviouslyTransparentActor);
		}
	}

	// 현재 프레임의 투명화된 오브젝트를 저장
	PreviouslyTransparentActors = CurrentlyTransparentActors;
}

void UCCameraManagerComponent::MakeActorTransparent(AActor* HitActor)
{
	CheckNull(HitActor);
	
	if (MaterialMementos.Contains(HitActor))
		return;
	
	// 상태 저장 (Memento 생성)
	auto Mesh = HitActor->FindComponentByClass<UStaticMeshComponent>();
	if (!Mesh) return;
	MaterialMementos.Add(HitActor, FMaterialMemento(Mesh));

	// 투명 메테리얼 적용
	for (int32 i = 0; i < Mesh->GetNumMaterials(); ++i)
	{
		Mesh->SetMaterial(i, TransparentMaterial);
	}
}

void UCCameraManagerComponent::ResetActorMaterial(AActor* Actor)
{
	CheckNull(Actor);

	if (!MaterialMementos.Contains(Actor))
		return;

	// 저장된 상태로 복구
	if (IsValid(Actor))
	{
		auto Mesh = Actor->FindComponentByClass<UStaticMeshComponent>();
		MaterialMementos[Actor].Restore(Mesh);
	}

	// 복구 후 삭제
	MaterialMementos.Remove(Actor);
}


// 카메라 전환
void UCCameraManagerComponent::ChangeToSubCamera(FName InName)
{
	CheckNull(OwnerCharacter);
	CheckNull(SubCameraActor);
	CheckNull(BlendDatas);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(OwnerCharacter->GetWorld(), 0);
	CheckNull(PlayerController);
	
	const FCameraBlendData* Data = BlendDatas->FindBlendDataByName(InName);
	CheckNull(Data);
	
	FVector locationOffset = OwnerCharacter->GetActorForwardVector() * Data->Location.X +
		OwnerCharacter->GetActorRightVector() * Data->Location.Y +
			FVector(0.0f,0.0f,Data->Location.Z);
	
	FVector location = OwnerCharacter->GetActorLocation() + locationOffset;
	
	FQuat base = OwnerCharacter->GetActorQuat();
	FQuat offset = Data->Rotation.Quaternion();
	FQuat final = base * offset;
	FRotator finalRotation = final.Rotator();
	
	SubCameraActor->SetActorLocation(location);
	SubCameraActor->SetActorRotation(finalRotation);
	
	SubCameraActor->SetActorHiddenInGame(false);  // 카메라 활성화

	float BlendTime = Data->BlendInTime;
	float ReturnTime = Data->ReturnTime;
	
	PlayerController->SetViewTargetWithBlend(SubCameraActor, BlendTime);

	OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(CameraResetTimer);
	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(CameraResetTimer, [this, BlendTime]()
	{
		ResetToMainCamera(BlendTime);  // 복귀 시 블렌드 타임 적용
	}, ReturnTime, false);
}

// 노티파이용 & Curve사용
void UCCameraManagerComponent::Notify_ChangeToSubCamera(FName InName)
{
	CheckNull(OwnerCharacter);
	CheckNull(SubCameraActor);
	CheckNull(BlendDatas);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(OwnerCharacter->GetWorld(), 0);
	CheckNull(PlayerController);
	
	const FCameraBlendData* Data = BlendDatas->FindBlendDataByName(InName);
	CheckNull(Data);

	//UCurveFloat* curve = Data->Curve;

	// 1. Location & Rotator Blend
	FVector locationOffset = OwnerCharacter->GetActorForwardVector() * Data->Location.X +
		OwnerCharacter->GetActorRightVector() * Data->Location.Y +
			FVector(0.0f,0.0f,Data->Location.Z);
	
	FVector location = OwnerCharacter->GetActorLocation() + locationOffset;
	
	FQuat base = OwnerCharacter->GetActorQuat();
	FQuat offset = Data->Rotation.Quaternion();
	FQuat final = base * offset;
	FRotator finalRotation = final.Rotator();
	
	SubCameraActor->SetActorLocation(location);
	SubCameraActor->SetActorRotation(finalRotation);
	
	SubCameraActor->SetActorHiddenInGame(false);

	// 2. TimeLine & Floatcurve사용으로 Location을 변경 및 회전은 캐릭터를 보게하기
	// 일단 보류 멋있을꺼같지가 않음
	
}

void UCCameraManagerComponent::ResetToMainCamera(float BlendTime)
{
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	
	PlayerController->SetViewTargetWithBlend(OwnerCharacter, BlendTime);
}

void UCCameraManagerComponent::StartZoom()
{
	if (!SpringArm) return;

	OriginalArmLength = SpringArm->TargetArmLength;
	bZoomingOut = true;

	// 일정 시간 후 복원
	GetWorld()->GetTimerManager().SetTimer(ZoomResetHandle, this, &UCCameraManagerComponent::ResetZoom, 1.0f, false);
}

void UCCameraManagerComponent::ResetZoom()
{
	bZoomingOut = false;
}

// 팀 전투 카메라 테스트 용 삭제 가능
void UCCameraManagerComponent::ChangeCamera()
{
	if(UnitTest1 == nullptr)
		UnitTest1 = FHelpers::FindActor<ACCameraMoveTest>(GetWorld());

	if(!!UnitTest1)
		UnitTest1->ChangeCamera();
}
