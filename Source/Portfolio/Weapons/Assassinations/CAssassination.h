#pragma once

#include "CoreMinimal.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "UObject/NoExportTypes.h"
#include "CAssassination.generated.h"

UCLASS()
class PORTFOLIO_API UCAssassination : public UObject
{
	GENERATED_BODY()

public:
	UCAssassination();
	
public:
	virtual void BeginPlay(class ACharacter* InOwner, const TArray<FExecutionData>& InDatas);
	virtual void Tick(float InDeltaTime) {};

public:
	virtual void DoAssassination();
	
protected:
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	
	// 움직임 제한하려고 가져옴
	class UCMoveComponent* Movement;
	// 상태 변경하려고 가져옴
	class UCStateComponent* State;
	// 암살 데이터
	TMap<EExecutionType, FExecutionData> ExecutionMap;

	
};