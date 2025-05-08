#include "../Weapons/CAttachment.h"
#include "../Global.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"

//#define CAttachmentDebug

ACAttachment::ACAttachment()
{
	PrimaryActorTick.bCanEverTick = true;
	FHelpers::CreateComponent<USceneComponent>(this,&Root,"Root");
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	OwnerCharacter->OnDestroyed.AddDynamic(this,&ACAttachment::OnDead);
	
	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true,children);

	for(USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);

		// 자식이 캡슐 아니라면 넘어가고
		if(shape == nullptr)
			continue;

		shape->OnComponentBeginOverlap.AddDynamic(this,&ACAttachment::OnComponentBeginOverlap);
		shape->OnComponentEndOverlap.AddDynamic(this,&ACAttachment::OnComponentEndOverlap);
		
		Collisions.Add(shape);
	}

	// 시작하면 비활성화
	OffCollision();
	
	Super::BeginPlay();
}

void ACAttachment::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bIsTrackingCollision == false) return;
	
	for (UShapeComponent* Shape : Collisions)
	{
		CreateCollisionTrace(Shape, 2.0f);
	}
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),InSocketName);
}

void ACAttachment::AttachToCollision(FName InCollisionName)
{
	// Fist 구현 시 추가
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckNull(Cast<ACharacter>(OtherActor));

	// TEST
	//FLog::Log(OtherActor->GetName());
	//FLog::Log(OwnerCharacter->GetName());
	
	// UCDoAction::OnAttachmentBeginOverlap 호출
	if(OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter,this,Cast<ACharacter>(OtherActor));
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);

	// UCDoAction::OnAttachmentEndOverlap 호출
	if(OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter,Cast<ACharacter>(OtherActor));
}

// 여기 콜리전 키기
void ACAttachment::OnCollision()
{
	// UCDoAction::OnAttachmentBeginCollision 실행
	if(OnAttachmentBeginCollision.IsBound())
		OnAttachmentBeginCollision.Broadcast();
	
	for(UShapeComponent* shape : Collisions)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		bIsTrackingCollision = true;
	}
}

// 여기 콜리전 끄기
void ACAttachment::OffCollision()
{
	// UCDoAction::OnAttachmentEndCollision 실행
	if(OnAttachmentEndCollision.IsBound())
		OnAttachmentEndCollision.Broadcast();
	
	for(UShapeComponent* shape : Collisions)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bIsTrackingCollision = false;
		LocationCenterStack.Empty();
		LocationTopStack.Empty();
	}
}

void ACAttachment::OnDead(class AActor* DestroyedActor)
{
	Destroy();
}

void ACAttachment::CreateCollisionTrace(UShapeComponent* ShapeComponent, float Duration)
{
	CheckNull(ShapeComponent);
	
	// 현재 콜리전 컴포넌트의 위치와 크기 가져오기
	FVector Location = ShapeComponent->GetComponentLocation();
	FRotator Rotation = ShapeComponent->GetComponentRotation();
	
	FVector PrevLocation = LocationCenterStack.Num() > 0 ? LocationCenterStack.Last() : Location;

	// 캡슐 컴포넌트인 경우 처리
	if (UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(ShapeComponent))
	{
		float CapsuleRadius = Capsule->GetUnscaledCapsuleRadius();
		float CapsuleHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
#ifdef CAttachmentDebug
		// 디버그 캡슐 그리기
		DrawDebugCapsule(
			GetWorld(),
			Location,                // 캡슐 중심 위치
			CapsuleHalfHeight,       // 캡슐 반높이
			CapsuleRadius,           // 캡슐 반지름
			Rotation.Quaternion(),   // 캡슐 회전
			FColor::Green,           // 디버그 색상
			false,                   // 영구 표시 여부
			Duration                 // 지속 시간
		);
#endif
		
	}
	
	// 위치가 다르다면..
	if(Location != PrevLocation)
	{
#ifdef CAttachmentDebug
		// 디버그 라인 그리기
		DrawDebugLine
		(
			GetWorld(),
			PrevLocation,              // 이전 위치
			Location,               // 현재 위치
			FColor::Green,
			false,                         // 영구 여부
			Duration,                      // 지속 시간
			0,                             // 선 우선순위
			2.0f                           // 두께
		);
#endif
		// LineTraceMultiByChannel 사용
		TArray<FHitResult> HitResults;
		bool bHit = GetWorld()->LineTraceMultiByChannel
		(
			HitResults,
			PrevLocation,
			Location,
			ECC_PhysicsBody,
			FCollisionQueryParams(FName(TEXT("PhysicsTrace")), false, this)
		);

		if(bHit)
		{
			for (const FHitResult& HitResult : HitResults)
			{
				AActor* OtherActor = HitResult.GetActor();
				if(OtherActor)
				{
					ACharacter* OtherCharacter  = Cast<ACharacter>(OtherActor);
#ifdef CAttachmentDebug
					FString str ="";
					str.Append("Trace Center");
					str.Append(OtherActor->GetName());
					FLog::Print(str);
#endif
					if(OwnerCharacter && OtherCharacter)
					{
						if(OnAttachmentBeginOverlap.IsBound())
							OnAttachmentBeginOverlap.Broadcast(OwnerCharacter,this,OtherCharacter);
					}
				}
			}
		}
	}
	// 위치 스택에 현재 위치 추가
	LocationCenterStack.Add(Location);
	
	//캡슐 윗쪽 계산
	UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(ShapeComponent);
	if(CapsuleComponent)
	{
		FVector UpVector = ShapeComponent->GetUpVector();
		FVector TopLocation = Location + (UpVector * CapsuleComponent->GetScaledCapsuleHalfHeight());
		
		FVector PrevTopLocation = LocationTopStack.Num()>0? LocationTopStack.Last() : TopLocation;

		if(TopLocation != PrevTopLocation)
		{
#ifdef CAttachmentDebug
			DrawDebugLine
			(
				GetWorld(),
				PrevTopLocation,               // 이전 위치
				TopLocation,				   // 현재 위치
				FColor::Green,
				false,                         // 영구 여부
				Duration,                      // 지속 시간
				0,                             // 선 우선순위
				2.0f                           // 두께
			);
#endif
			// LineTraceMultiByChannel 사용
			TArray<FHitResult> HitResults;
			bool bHit = GetWorld()->LineTraceMultiByChannel
			(
				HitResults,
				PrevTopLocation,
				TopLocation,
				ECC_PhysicsBody,
				FCollisionQueryParams(FName(TEXT("PhysicsTrace")), false, this)
			);

			if(bHit)
			{
				for (const FHitResult& HitResult : HitResults)
				{
					AActor* OtherActor = HitResult.GetActor();
					if(OtherActor)
					{
						ACharacter* OtherCharacter  = Cast<ACharacter>(OtherActor);
#ifdef CAttachmentDebug
						FString str ="";
						str.Append("Trace Top");
						str.Append(OtherActor->GetName());
						FLog::Print(str);
#endif
						if (OwnerCharacter && OtherCharacter)
						{
							if(OnAttachmentBeginOverlap.IsBound())
								OnAttachmentBeginOverlap.Broadcast(OwnerCharacter,this,OtherCharacter);
						}
					}
				}
			}
		}
		// 위치 스택에 현재 위치 추가
		LocationTopStack.Add(TopLocation);
	}
	
}
