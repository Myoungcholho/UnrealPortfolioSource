#include "../BehaviorTree/CBTTaskNode_Patrol.h"

#include "CPatrolPath.h"
#include "../Global.h"
#include "Portfolio/Components/CAIBehaviorComponent.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Portfolio/Components/CMoveComponent.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName ="Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(controller);
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCMoveComponent* movement = FHelpers::GetComponent<UCMoveComponent>(ai);
	if(movement != nullptr)
		movement->EnableControlRotation();
	
	ACPatrolPath* patrolPath = ai->GetPatrolPath();
	if(!!patrolPath)
	{
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);
		if(bDebug)
			DrawDebugSphere(ai->GetWorld(),moveToPoint,10,40,FColor::Green,false,20);
		return EBTNodeResult::InProgress;
	}


	FVector location = ai->GetActorLocation();
	UNavigationSystemV1* navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navigation, EBTNodeResult::Failed);

	FNavLocation point(location);

	navigation->GetRandomPointInNavigableRadius(location, RandomRadius,point);


	/*리펙토링 대상 -> 비동기 호출*/
	int maxAttempts = 100;
	int attempts = 0;
	while(attempts < maxAttempts)
	{
		if(navigation->GetRandomPointInNavigableRadius(location,RandomRadius,point))
			break;

		++attempts;
	}
	if (attempts >= maxAttempts)
		return EBTNodeResult::Failed;

	behavior->SetPatrolLocation(point.Location);

	if(bDebug)
		DrawDebugSphere(ai->GetWorld(),point.Location,10,40,FColor::Green,false,20);
	
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	
	FVector location = behavior->GetPatrolLocation();

	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location,GoalDistance,false);
	
	switch(result)
	{
	case EPathFollowingRequestResult::Failed:
		{
			// 갈수 없는 구역이면 끝내기
			FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		}
		break;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		{
			// 도달했다면 성공으로 끝내기 + Point있다면 다음으로
			if(!!ai->GetPatrolPath())
				ai->GetPatrolPath()->UpdateIndex();
			
			FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		}
		break;
	}
	
}

/*EBTNodeResult::Type UCBTTaskNode_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	//UCMoveComponent* movement = FHelpers::GetComponent<UCMoveComponent>(ai);
	//if(movement != nullptr)
		//movement->DisableControlRotation();
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}*/
