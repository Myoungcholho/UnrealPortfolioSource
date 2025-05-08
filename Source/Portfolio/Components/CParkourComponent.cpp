#include "../Components/CParkourComponent.h"
#include "../Global.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UCParkourComponent::UCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FHelpers::GetAsset<UDataTable>(&DataTable, "/Script/Engine.DataTable'/Game/Parkour/DT_Parkour.DT_Parkour'");
}

void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);


	USceneComponent* arrows = FHelpers::GetComponent<USceneComponent>(OwnerCharacter, "Arrows");
	CheckNull(arrows);

	TArray<USceneComponent*> components;
	arrows->GetChildrenComponents(false, components);

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		Arrows[i] = Cast<UArrowComponent>(components[i]);

		ensure(Arrows[i] != nullptr);
	}

	
	TArray<FParkourData*> rows;
	DataTable->GetAllRows<FParkourData>("", rows);

	for (int i = 0; i < (int32)EParkourType::Max; i++)
	{
		TArray<FParkourData> temp;

		for (FParkourData* data : rows)
		{
			if (data->Type == (EParkourType)i)
				temp.Add(*data);
		}

		DataMap.Add((EParkourType)i, temp);
	}
}

void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//CheckTrace_Land();
}

void UCParkourComponent::DoParkour(bool bLanded)
{
	CheckFalse(Type == EParkourType::Max);

	/*if (bLanded && Check_FallMode())
	{
		DoParkour_Fall();

		return;
	}*/


	HitObstacle = nullptr;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0.0f;
	ToFrontYaw = 0.0f;

	
	CheckTrace_Center();

	if (!!HitObstacle)
	{
		LineTrace(EParkourArrowType::Head);
		LineTrace(EParkourArrowType::Foot);
		LineTrace(EParkourArrowType::Left);
		LineTrace(EParkourArrowType::Right);
	}

	CheckFalse(Check_Obstacle());


	/*if (Check_ClimbMode())
	{
		DoParkour_Climb();

		return;
	}*/

	if (Check_SlideMode())
	{
		DoParkour_Slide();
		
		return;
	}


	FParkourData data;

	/*if (Check_ObstacleMode(EParkourType::Normal, data))
	{
		DoParkour_Obstacle(EParkourType::Normal, data);

		return;
	}

	if (Check_ObstacleMode(EParkourType::Short, data))
	{
		DoParkour_Obstacle(EParkourType::Short, data);

		return;
	}*/

	if (Check_ObstacleMode(EParkourType::Wall, data))
	{
		DoParkour_Obstacle(EParkourType::Wall, data);

		return;
	}
}

void UCParkourComponent::End_DoParkour()
{
	switch (Type)
	{
	case EParkourType::Climb:
		End_DoParkour_Climb();
		break;

	case EParkourType::Slide:
		End_DoParkour_Slide();
		break;

	case EParkourType::Short:
	case EParkourType::Normal:
	case EParkourType::Wall:
		End_DoParkour_Obstacle();
		break;
	}

	Type = EParkourType::Max;
}

void UCParkourComponent::LineTrace(EParkourArrowType InType, float InLineDistance)
{
	UArrowComponent* arrow = Arrows[(int32)InType];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	
	float distance = TraceDistance;
	
	if (InLineDistance > 0.0f)
		distance = InLineDistance;

	
	FTransform transform = arrow->GetComponentToWorld();

	FVector start = transform.GetLocation();
	FVector end = start + transform.GetRotation().GetForwardVector() * distance;


	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);


	ETraceTypeQuery type = ETraceTypeQuery::TraceTypeQuery3;

	if(InType == EParkourArrowType::Land)
		type = ETraceTypeQuery::TraceTypeQuery1;


	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, type, false, ignores, DrawDebug, HitResults[(int32)InType], true, color, FLinearColor::White);
}

void UCParkourComponent::CheckTrace_Center()
{
	EParkourArrowType type = EParkourArrowType::Center;
	LineTrace(type);


	const FHitResult& hitResult = HitResults[(int)type];
	CheckFalse(hitResult.bBlockingHit);


	UStaticMeshComponent* mesh = FHelpers::GetComponent<UStaticMeshComponent>(hitResult.GetActor());
	CheckNull(mesh);


	HitObstacle = hitResult.GetActor();

	FVector minBound, maxBound;
	mesh->GetLocalBounds(minBound, maxBound);

	float x = FMath::Abs(minBound.X - maxBound.X);
	float y = FMath::Abs(minBound.Y - maxBound.Y);
	float z = FMath::Abs(minBound.Z - maxBound.Z);
	HitObstacleExtent = FVector(x, y, z);

	HitDistance = hitResult.Distance;
	
	ToFrontYaw = UKismetMathLibrary::MakeRotFromX(-hitResult.ImpactNormal).Yaw;
}

void UCParkourComponent::CheckTrace_Land()
{
	CheckFalse(OwnerCharacter->GetCharacterMovement()->IsFalling());
	CheckTrue(bBeginFalling);

	bBeginFalling = true;
	EParkourArrowType type = EParkourArrowType::Land;

	LineTrace(type, (*DataMap.Find(EParkourType::Fall))[0].MaxDistance);
}

bool UCParkourComponent::Check_Obstacle()
{
	CheckNullResult(HitObstacle, false);

	
	bool b = true;
	b &= HitResults[(int32)EParkourArrowType::Center].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Left].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Right].bBlockingHit;
	CheckFalseResult(b, false);


	FVector center = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;
	FVector left = HitResults[(int32)EParkourArrowType::Left].ImpactNormal;
	FVector right = HitResults[(int32)EParkourArrowType::Right].ImpactNormal;

	CheckFalseResult(center.Equals(left), false);
	CheckFalseResult(center.Equals(right), false);


	FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint;
	FVector end = OwnerCharacter->GetActorLocation();

	float lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;
	float impactAt = UKismetMathLibrary::MakeRotFromX(center).Yaw;
	
	float yaw = FMath::Abs(FMath::Abs(lookAt) - FMath::Abs(impactAt));
	
	CheckFalseResult(yaw <= AvailableFrontAngle, false);

	return true;
}

bool UCParkourComponent::Check_ClimbMode()
{
	CheckFalseResult(HitResults[(int32)EParkourArrowType::Head].bBlockingHit, false);


	TArray<FParkourData>* data = DataMap.Find(EParkourType::Climb);
	if(data == nullptr)
		return false;
	
	CheckFalseResult((*data)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*data)[0].MaxDistance > HitDistance, false);
	CheckFalseResult(FMath::IsNearlyEqual((*data)[0].Extent, HitObstacleExtent.Z, 10), false);

	return true;
}

void UCParkourComponent::DoParkour_Climb()
{
	Type = EParkourType::Climb;


	OwnerCharacter->SetActorLocation(HitResults[(int32)EParkourArrowType::Center].ImpactPoint);

	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));


	(*DataMap.Find(EParkourType::Climb))[0].PlayMontage(OwnerCharacter);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UCParkourComponent::End_DoParkour_Climb()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

bool UCParkourComponent::Check_FallMode()
{
	CheckFalseResult(bBeginFalling, false);
	bBeginFalling = false;


	float fallDistance = HitResults[(int32)EParkourArrowType::Land].Distance;
	float extent = (*DataMap.Find(EParkourType::Fall))[0].Extent;

	CheckFalseResult(extent < fallDistance, false);


	return true;
}

void UCParkourComponent::DoParkour_Fall()
{
	Type = EParkourType::Fall;

	(*DataMap.Find(EParkourType::Fall))[0].PlayMontage(OwnerCharacter);
}

bool UCParkourComponent::Check_SlideMode()
{
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Foot].bBlockingHit, false);
    
    const TArray<FParkourData>* data = DataMap.Find(EParkourType::Slide);
    CheckFalseResult((*data)[0].MinDistance < HitDistance, false);
    CheckFalseResult((*data)[0].MaxDistance > HitDistance, false);


    UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Foot];
    FLinearColor color = FLinearColor(arrow->ArrowColor);


    FTransform transform = arrow->GetComponentToWorld();

    FVector arrowLocation = transform.GetLocation();
    FVector ownerLocation = OwnerCharacter->GetActorLocation();


    float extend = (*data)[0].Extent;
    float z = arrowLocation.Z + extend;

    FVector forward = OwnerCharacter->GetActorForwardVector();
    forward = FVector(forward.X, forward.Y, 0.0f);

    FVector start = FVector(arrowLocation.X, arrowLocation.Y, z);
    FVector end = start + forward * TraceDistance;


    TArray<AActor*> ignores;
    ignores.Add(OwnerCharacter);

    FHitResult hitResult;

    UKismetSystemLibrary::BoxTraceSingle(GetWorld(), start, end, FVector(0, extend, extend), OwnerCharacter->GetActorRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ignores, DrawDebug, hitResult, true, color, FLinearColor::White);
    CheckTrueResult(hitResult.bBlockingHit, false);

    return true;
}

void UCParkourComponent::DoParkour_Slide()
{
	Type = EParkourType::Slide;

	(*DataMap.Find(EParkourType::Slide))[0].PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_Slide()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = nullptr;
}

bool UCParkourComponent::Check_ObstacleMode(EParkourType InType, FParkourData& OutData)
{
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Head].bBlockingHit, false);

	const TArray<FParkourData>* data = DataMap.Find(InType);

	for (int32 i = 0; i < (*data).Num(); i++)
	{
		bool b = true;
		b &= (*data)[i].MinDistance < HitDistance;
		b &= (*data)[i].MaxDistance > HitDistance;
		b &= FMath::IsNearlyEqual((*data)[i].Extent, HitObstacleExtent.Y, 10);

		OutData = (*data)[i];
		CheckTrueResult(b, true);
	}

	return false;
}

void UCParkourComponent::DoParkour_Obstacle(EParkourType InType, FParkourData& InData)
{
	Type = InType;

	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));

	InData.PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_Obstacle()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = nullptr;
}



