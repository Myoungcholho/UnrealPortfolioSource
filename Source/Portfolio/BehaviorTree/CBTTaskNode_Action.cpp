#include "../BehaviorTree/CBTTaskNode_Action.h"

#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"
#include "Portfolio/Utilities/FHelpers.h"
#include "../Weapons/DoActions/CDoAction.h"
#include "Portfolio/Components/CStateComponent.h"
#include "Portfolio/Utilities/FLog.h"
#include "Portfolio/Weapons/CEquipment.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName ="Action";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon,EBTNodeResult::Failed);
	CheckTrueResult(weapon->IsUnarmedMode(),EBTNodeResult::Failed);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);
	CheckNullResult(state,EBTNodeResult::InProgress);

	if(state->IsDeadMode())
		return EBTNodeResult::InProgress;
	
	controller->StopMovement();
	weapon->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(state);
	CheckNull(weapon);
	CheckTrue(weapon->IsUnarmedMode());

	if(state->IsDeadMode() == true)
		return;
	
	//거리가 가까우면 DoAction 실행
	//FLog::Log("call tick do Action");
	weapon->DoAction();
	
	bool bCheck = true;
	bCheck &= state->IsIdleMode();
	bCheck &= weapon->GetDoAction()->GetInAction() == false;
	
	if(bCheck)
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon,EBTNodeResult::Aborted);
	
	bool bBeginAction = weapon->GetDoAction()->GetBeginAction();
	if(bBeginAction == false)
		weapon->GetDoAction()->Begin_DoAction();
	
	weapon->GetDoAction()->End_DoAction(); // 내부에서 Idle 모드로 변경 중
	// 여기에서 만약 Damage받아서 호출된것이라면 다시 Damage 모드로 변경함
	return EBTNodeResult::Aborted;
}
