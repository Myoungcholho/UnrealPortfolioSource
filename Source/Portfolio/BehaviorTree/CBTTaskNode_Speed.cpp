#include "../BehaviorTree/CBTTaskNode_Speed.h"
#include "../Global.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName="Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	
	UCMoveComponent* movement = FHelpers::GetComponent<UCMoveComponent>(ai);

	switch (Type)
	{
		case ESpeedType::Walk: movement->OnWalk(); break;
		case ESpeedType::Run: movement->OnRun(); break;
		case ESpeedType::Sprint: movement->OnSprint(); break;
	}
	
	return EBTNodeResult::Succeeded;
}
