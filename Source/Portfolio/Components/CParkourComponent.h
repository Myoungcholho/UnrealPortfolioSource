#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../GameEnums/CParkourEnum.h"
#include "CParkourComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* DataTable;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance = 600;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float AvailableFrontAngle = 15;

public:
	FORCEINLINE bool IsDoing() { return Type != EParkourType::Max; }
	
public:	
	UCParkourComponent();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void DoParkour(bool bLanded = false);
	void End_DoParkour();

private:
	void LineTrace(EParkourArrowType InType, float InLineDistance = 0.0f);
	void CheckTrace_Center();
	void CheckTrace_Land();

private:
	bool Check_Obstacle();

private:
	bool Check_ClimbMode();
	void DoParkour_Climb();
	void End_DoParkour_Climb();

private:
	bool Check_FallMode();
	void DoParkour_Fall();

private:
	bool Check_SlideMode();
	void DoParkour_Slide();
	void End_DoParkour_Slide();

private:
	bool Check_ObstacleMode(EParkourType InType, FParkourData& OutData);
	void DoParkour_Obstacle(EParkourType InType, FParkourData& InData);
	void End_DoParkour_Obstacle();

private:
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];
	FHitResult HitResults[(int32)EParkourArrowType::Max];

private:
	class ACharacter* OwnerCharacter;

private:
	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float HitDistance;
	float ToFrontYaw;

private:
	TMap<EParkourType, TArray<FParkourData>> DataMap;

private:
	EParkourType Type = EParkourType::Max;

private:
	bool bBeginFalling;

private:
	AActor* BackupObstacle;
		
};
