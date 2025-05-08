#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_TargetLookAt.generated.h"

UCLASS()
class PORTFOLIO_API UCBTTaskNode_TargetLookAt : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_TargetLookAt();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "LookAt")
	float RotationSpeed = 5.0f;
	
};
