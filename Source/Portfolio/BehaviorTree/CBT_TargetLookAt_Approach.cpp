#include "../BehaviorTree/CBT_TargetLookAt_Approach.h"
#include "../Global.h"
#include "Portfolio/Components/CAIBehaviorComponent.h"
#include "Portfolio/Enemies/CAIController.h"

UCBT_TargetLookAt_Approach::UCBT_TargetLookAt_Approach()
{
	NodeName = "LookAt_Approach";
}

/*EBTNodeResult::Type UCBT_TargetLookAt_Approach::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller,EBTNodeResult::Aborted);

	UCAIBehaviorComponent* behavior = controller->GetAIBehavior();
	CheckNullResult(behavior,EBTNodeResult::Aborted);

	int temp = behavior->GetAttackableCount();

	if(temp - 1 >= 0)
	{
		behavior->SetAttackableCount(temp -1);
		FLog::Log("Abort Approach");
	}
	
	return EBTNodeResult::Aborted;
}*/
