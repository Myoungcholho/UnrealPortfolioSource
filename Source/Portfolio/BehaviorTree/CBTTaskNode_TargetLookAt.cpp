#include "../BehaviorTree/CBTTaskNode_TargetLookAt.h"
#include "../Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "Portfolio/Components/CAIBehaviorComponent.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"

UCBTTaskNode_TargetLookAt::UCBTTaskNode_TargetLookAt()
{
	NodeName = "LookAt";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_TargetLookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller,EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai,EBTNodeResult::InProgress);

	ACharacter* character = Cast<ACharacter>(ai);

	character->bUseControllerRotationYaw = true;
	character->GetCharacterMovement()->bOrientRotationToMovement = false;
	/////////////////////////////////////////////////
	UCAIBehaviorComponent* behavior = controller->GetAIBehavior();
	CheckNullResult(behavior,EBTNodeResult::Failed);

	ACharacter* target = behavior->GetTarget();
	CheckNullResult(target,EBTNodeResult::Failed);
	
	controller->SetFocus(target);
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_TargetLookAt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	/*ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCAIBehaviorComponent* behavior = controller->GetAIBehavior();
	CheckNull(behavior);

	ACharacter* target = behavior->GetTarget();
	CheckNull(target);

	controller->SetFocus(target); // ✅ 매 프레임 다시 지정해줌*/
}

EBTNodeResult::Type UCBTTaskNode_TargetLookAt::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller,EBTNodeResult::Aborted);

	controller->ClearFocus(EAIFocusPriority::Gameplay);
	
	return EBTNodeResult::Aborted;
}
