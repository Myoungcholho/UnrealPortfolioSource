#include "../BehaviorTree/CBTTaskNode_BossAction.h"
#include "../Global.h"
#include "Portfolio/Components/CAIBehaviorComponent.h"
#include "Portfolio/Components/CWeaponComponent.h"

#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"

UCBTTaskNode_BossAction::UCBTTaskNode_BossAction()
{
	NodeName = "BossAction";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_BossAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(controller);
	CheckNullResult(behavior, EBTNodeResult::Failed);
	
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon,EBTNodeResult::Failed);
	CheckTrueResult(weapon->IsUnarmedMode(),EBTNodeResult::Failed);


	// 거리 확인
	ACharacter* target = behavior->GetTarget();
	CheckNullResult(target,EBTNodeResult::Failed);
	
	if(IsPlayIndex)
	{
		//FLog::Log(PlayIndexAction);
		weapon->DoAction(PlayIndexAction);
		return EBTNodeResult::InProgress;
	}

	float DistanceToTarget = FVector::Dist(ai->GetActorLocation(), target->GetActorLocation());
	FLog::Log(DistanceToTarget);
	if(DistanceToTarget < MeleeAttackDistance)
	{
		int32 playPattern = FMath::RandRange(PatternMin,PatternMax);

		//FLog::Log(playPattern);
		//FLog::Log(DistanceToTarget);
		FLog::Log(playPattern);
		weapon->DoAction(playPattern);
	}
	else
	{
		int32 playPattern = FMath::RandRange(PatternMax-1,PatternMax);
		FLog::Log(playPattern);
		weapon->DoAction(playPattern);
	}
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_BossAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UCBTTaskNode_BossAction::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
