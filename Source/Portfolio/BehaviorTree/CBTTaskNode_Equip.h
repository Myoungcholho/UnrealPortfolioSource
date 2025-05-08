#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "CBTTaskNode_Equip.generated.h"

UCLASS()
class PORTFOLIO_API UCBTTaskNode_Equip : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category="Weapon")
	EWeaponType Type = EWeaponType::Max;
	
public:
	UCBTTaskNode_Equip();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
