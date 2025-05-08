#include "../BehaviorTree/CBTService_Boss.h"
#include "../Global.h"
#include "Portfolio/Components/CAIBehaviorComponent.h"
#include "Portfolio/Components/CMoveComponent.h"

#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"
#include "Portfolio/Weapons/CEquipment.h"

UCBTService_Boss::UCBTService_Boss()
{
	NodeName = "Boss";
	
	Interval = 0.1f;
	RandomDeviation = 0.0f;
	
	ActionCooldown =0.0f;

	ActionInterval.X = 5.0f;
	ActionInterval.Y = 10.0f;
}

void UCBTService_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNull(weapon);
	
	ACharacter* target = behavior->GetTarget();
	CheckNull(target);

	UCMoveComponent* movement = FHelpers::GetComponent<UCMoveComponent>(ai);
	CheckNull(movement);
	
	if(movement->IsControlRotationPending())
	{
		//FLog::Log("RETURN SERIVCE, Because Pending");
		behavior->SetWaitMode();
		return;
	}
	
	// 1. Action 중에는 Service 정지
	if(CanProvideService(OwnerComp) == false)
	{
		//FLog::Log("RETURN SERIVCE, Because Action");
		return;
	}
	
	if(weapon->GetEquipment() == nullptr)
	{
		behavior->SetEquipMode();
		
		return;
	}
	bool bEquipped = *(weapon->GetEquipment()->GetEquipped());
	CheckFalse(bEquipped);

	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	behavior->SetStrafeMode();
	
	float LastAttackTime = behavior->GetActionTime();
	if (CurrentTime >= LastAttackTime + ActionCooldown) 
	{
		behavior->SetActionMode();
		ActionCooldown = FMath::RandRange(ActionInterval.X, ActionInterval.Y);
	}
}

bool UCBTService_Boss::CanProvideService(UBehaviorTreeComponent& OwnerComp)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* State = FHelpers::GetComponent<UCStateComponent>(ai);
	CheckNullResult(State,false);

	if(State->IsActionMode())
		return false;

	if(State->IsGuardParryMode())
		return false;

	if(State->IsDeadMode())
		return false;
	if(State->IsDamagedMode())
		return false;
	
	return true;
}
