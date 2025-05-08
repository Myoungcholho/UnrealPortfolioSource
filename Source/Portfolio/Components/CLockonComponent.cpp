#include "../Components/CLockonComponent.h"
#include "../Global.h"

#include "CMoveComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

//#define DebugLockon

UCLockonComponent::UCLockonComponent()
{
	ActorClassFilter = AActor::StaticClass();
}

void UCLockonComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(OwnerCharacter == nullptr) return;

	Movement = FHelpers::GetComponent<UCMoveComponent>(OwnerCharacter);
	CheckNull(Movement);
}

class AActor* UCLockonComponent::GetTargetInFront()
{
	CheckNullResult(OwnerCharacter,nullptr);

	// 1. 액터 전부 얻어오기
	TArray<AActor*> FoundActors;
	FVector Center = OwnerCharacter->GetActorLocation();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerCharacter);
	
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		Radius,
		ObjectTypes, // 검색할 타입 Pawn
		ActorClassFilter, // 필터 CEnemy 
		IgnoreActors,
		FoundActors
	);
#ifdef DebugLockon
	DrawDebugSphere
	(
		GetWorld(),
		Center,              // 구 중심
		Radius,              // 구 반경
		16,                  // 세그먼트 수
		FColor::Green,       // 색상
		false,               // 지속 여부
		5.0f                 // 지속 시간
	);
#endif

#ifdef DebugLockon
	if(bHit)
	{
		for (AActor* Actor : FoundActors)
		{
			// 액터 위치에 디버그 포인트 표시
			DrawDebugSphere
			(
				GetWorld(),                     // 월드
				Actor->GetActorLocation(),      // 구체 중심 위치
				10.0f,                         // 반지름 (100.0f)
				16,                             // 세그먼트 개수 (구체의 해상도)
				FColor::Red,                    // 색상
				false,                          // 지속 여부 (true: 영구, false: 일정 시간 후 사라짐)
				3.0f,                           // 지속 시간 (5초 후 사라짐)
				0,                              // 깊이 우선순위 (0: 기본)
				2.0f                            // 선 두께
			);
		}
	}
#endif
	
	CheckFalseResult(bHit,nullptr);

	// 2. 후보 탐색
	AActor* ClosestActor = nullptr;
	float ClosestDistance = Radius;

	FVector ForwardVector = OwnerCharacter->GetActorForwardVector();
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();

	for (int32 i = FoundActors.Num() - 1; i >= 0; --i)
	{
		AActor* Actor = FoundActors[i];
		if (Actor == nullptr) continue;

		FVector ToTarget = Actor->GetActorLocation() - OwnerLocation;
		ToTarget.Normalize();
		
		float DotProduct = FVector::DotProduct(ForwardVector, ToTarget);
		float AngleInDegrees = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

#ifdef DebugLockon
		if (AngleInDegrees <= LimitAngle)
		{
			DrawDebugLine
			(
				GetWorld(),
				OwnerLocation,
				Actor->GetActorLocation(),
				FColor::Blue,  // 시야각 내에 있는 경우 파란색 선
				false,
				3.0f,
				0,
				1.0f
			);
		}
#endif
		
		if (AngleInDegrees > LimitAngle)
		{
			FoundActors.RemoveAt(i);
			continue;
		}

		FHitResult HitResult;
		FVector Start = OwnerLocation;
		FVector End = Actor->GetActorLocation();

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(OwnerCharacter); // 본인 제외
		TraceParams.AddIgnoredActor(Actor);         // 현재 목표 제외

		// LineTraceSingleByChannel 실행
		bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility, // 충돌 채널 (시야로 설정)
			TraceParams
		);

		if (bBlockingHit)
		{
			// 장애물이 있으면 해당 액터를 후보에서 제거
			FoundActors.RemoveAt(i);
			continue;
		}
		
		
		// 가장 가까운 액터를 찾습니다.
		float DistanceToActor = FVector::Dist(OwnerLocation, Actor->GetActorLocation());
		if (DistanceToActor < ClosestDistance)
		{
			ClosestActor = Actor;
			ClosestDistance = DistanceToActor;
		}
		
	}


	// 3.확인용
#ifdef DebugLockon
	if(!!ClosestActor)
	{
		DrawDebugLine
	   (
		   GetWorld(),
		   OwnerLocation,
		   ClosestActor->GetActorLocation(),
		   FColor::Magenta,
		   false,
		   5.0f,
		   0,
		   2.0f
	   );
	}
#endif
	
		
	return ClosestActor;
}

void UCLockonComponent::ChangeLockMode()
{
	GetTargetInFront();
	
	CheckNull(Movement);
	bLockOn = !bLockOn;

	if(bLockOn)
		Movement->EnableControlRotation();
	else
		Movement->DisableControlRotation();
}

void UCLockonComponent::OnLock()
{
	CheckNull(Movement);
	bLockOn = true;
	//Movement->EnableControlRotation();
	//FLog::Log("LockOn!");
}

void UCLockonComponent::OffLock()
{
	CheckNull(Movement);
	bLockOn = false;
	//Movement->DisableControlRotation();
	//FLog::Log("Lockoff!");
}
