#include "../Components/CFeetComponent.h"
#include "../Global.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"

//#define LOG_UCFeetComponent

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);
	
	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
}

void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckNull(State);
	/*if(State->IsIdleMode() == false)
		return;*/
	
	float leftDistance;
	FRotator leftRotation;
	TraceBone(LeftBoneName, leftDistance, leftRotation);
	
	float rightDistance;
	FRotator rightRotation;
	TraceBone(RightBoneName, rightDistance, rightRotation);
	
	float pelvisDistance = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, pelvisDistance, DeltaTime, InterpSpeed);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - pelvisDistance), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - pelvisDistance), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);

#ifdef LOG_UCFeetComponent
	FLog::Print(Data.LeftDistance, 9990);
	FLog::Print(Data.RightDistance, 9991);
	FLog::Print(Data.PelvisDistance, 9992);
	//FLog::Print(Data.LeftRotation, 9993);
	//FLog::Print(Data.RightRotation, 9994);
	//FLog::Print(rightRotation,9990);
	//FLog::Print(RightRotation_ball,9991);
#endif
}

void UCFeetComponent::TraceBone(FName InName, float& OutDistance, FRotator& OutRotation)
{
	FVector socketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InName);
	float z = OwnerCharacter->GetActorLocation().Z;

	FVector baseStart = FVector(socketLocation.X, socketLocation.Y , z);

	z = baseStart.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector baseEnd = FVector(baseStart.X, baseStart.Y, z);

	// 거리는 중심값 하나만 사용
	float centerDistance;
	FRotator centerRotation;
	Trace(baseStart, baseEnd, centerDistance, centerRotation);
	OutDistance = centerDistance;

	// 회전 오프셋
	TArray<FVector> Offsets = {
		FVector(0, 0, 0),
		FVector(8, 0, 0),
		FVector(-8, 0, 0),
		FVector(0, 8, 0),
		FVector(0, -8, 0)
	};

	TArray<float> Pitches;
	TArray<float> Rolls;
	
	for (const FVector& Offset : Offsets)
	{
		FVector start = baseStart + Offset;
		FVector end = baseEnd + Offset;

		float dummyDistance;
		FRotator oneRotation;
		Trace(start, end, dummyDistance, oneRotation);

		if (!oneRotation.IsNearlyZero())
		{
			Pitches.Add(oneRotation.Pitch);
			Rolls.Add(oneRotation.Roll);
		}
	}

	// 평균 계산
	if (Pitches.Num() > 0)
	{
		// 1차 평균
		float avgPitch = 0.0f;
		float avgRoll = 0.0f;
		for (int32 i = 0; i < Pitches.Num(); ++i)
		{
			avgPitch += Pitches[i];
			avgRoll += Rolls[i];
		}
		avgPitch /= Pitches.Num();
		avgRoll /= Rolls.Num();

		// 기준보다 벗어난 값 제거 (예: 15도 이상)
		TArray<float> filteredPitches;
		TArray<float> filteredRolls;

		const float Threshold = 15.0f;

		for (int32 i = 0; i < Pitches.Num(); ++i)
		{
			if (FMath::Abs(Pitches[i] - avgPitch) <= Threshold &&
				FMath::Abs(Rolls[i] - avgRoll) <= Threshold)
			{
				filteredPitches.Add(Pitches[i]);
				filteredRolls.Add(Rolls[i]);
			}
		}

		// 최종 평균
		if (filteredPitches.Num() > 0)
		{
			float finalPitch = 0.0f;
			float finalRoll = 0.0f;

			for (int32 i = 0; i < filteredPitches.Num(); ++i)
			{
				finalPitch += filteredPitches[i];
				finalRoll += filteredRolls[i];
			}

			finalPitch /= filteredPitches.Num();
			finalRoll /= filteredRolls.Num();

			OutRotation = FRotator(finalPitch, 0.0f, finalRoll);
		}
		else
		{
			OutRotation = FRotator::ZeroRotator;
		}
	}
	else
	{
		OutRotation = FRotator::ZeroRotator;
	}
	
}

void UCFeetComponent::Trace(FVector InStart, FVector InEnd, float& OutDistance, FRotator& OutRotation)
{
	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		InStart,
		InEnd,
		ETraceTypeQuery::TraceTypeQuery1,
		true,
		ignores,
		DrawDebug,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	OutDistance = 0.0f;
	OutRotation = FRotator::ZeroRotator;
	
	CheckFalse(hitResult.bBlockingHit);

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = length + OffsetDistance - TraceDistance;

	float roll = UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z);

	OutRotation = FRotator(pitch, 0.0f, roll);
}



