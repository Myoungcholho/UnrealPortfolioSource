#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Portfolio/Components/CMoveComponent.h"
#include "CBTTaskNode_Speed.generated.h"

UCLASS()
class PORTFOLIO_API UCBTTaskNode_Speed : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category="Type")
	ESpeedType Type;

public:
	UCBTTaskNode_Speed();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
