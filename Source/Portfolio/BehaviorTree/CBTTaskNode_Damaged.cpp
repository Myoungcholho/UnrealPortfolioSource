#include "../BehaviorTree/CBTTaskNode_Damaged.h"
#include "../Global.h"
#include "Portfolio/Components/CStateComponent.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"


UCBTTaskNode_Damaged::UCBTTaskNode_Damaged()
{
	NodeName = "Damaged";
	
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Damaged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());

	controller->StopMovement();
	// 여기에서 State-> damage로 다시 변경
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Damaged::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);

	// 여기 들어왔을때는 데미지 모드 설정되어 있음
	// 근데 아니라면 끝난것임.
	if (state->IsDamagedMode() == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}