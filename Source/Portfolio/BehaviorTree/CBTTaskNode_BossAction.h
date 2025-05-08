#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_BossAction.generated.h"

UCLASS()
class PORTFOLIO_API UCBTTaskNode_BossAction : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category="Action")
	int PatternMin;
	
	UPROPERTY(EditAnywhere, Category="Action")
	int PatternMax;

	UPROPERTY(EditAnywhere, Category="Action")
	float MeleeAttackDistance = 400.0f;
	
	// 0~6는 근접 공격
	UPROPERTY(EditAnywhere, Category="Test")
	bool IsPlayIndex;
	
	UPROPERTY(EditAnywhere,Category="Test")
	int32 PlayIndexAction;
	
public:
	UCBTTaskNode_BossAction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
