#include "../BehaviorTree/CBTService_Melee.h"
#include "../Global.h"
#include "Portfolio/Components/CAIBehaviorComponent.h"
#include "Portfolio/Components/CStateComponent.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"
#include "Portfolio/Managers/CGameInstance.h"
#include "Portfolio/Managers/CTeamCombatManager.h"
#include "Portfolio/Weapons/CEquipment.h"


UCBTService_Melee::UCBTService_Melee()
{
	NodeName ="Melee";

	// 0.1초마다 서비스 콜
	Interval = 0.1f;
	// 랜덤편차 없애줌
	RandomDeviation = 0.0f;
}

///////////////////////////////////////////
// 1. 순찰하다가
// 2. 대상을 발견하면 장착
// 3. 장착 되었다면 스트레이프 대기
// 4. 대기 중 블랙보드에서 값을 얻어 접근 대상 선택
///////////////////////////////////////////
void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	
	ACharacter* target = behavior->GetTarget();

	UCStateComponent* State = FHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(State);

	UCGameInstance* gameInstance = Cast<UCGameInstance>(ai->GetOwner()->GetGameInstance());
	CheckNull(gameInstance);

	UCTeamCombatManager* team = gameInstance->GetTeamCombatManager();
	CheckNull(team);
	
	if(State->IsDeadMode())
		return;
	if(State->IsDamagedMode())
		return;
	if(State->IsGuardImpactMode())
		return;
	if(State->IsActionMode())
		return;
	if(State->IsGuardParryMode())
		return;
	if(State->IsGuardBreakMode())
		return;

	
	if(target == nullptr)
	{
		if(WaitTrue)
		{
			behavior->SetWaitMode();
		}
		else
		{
			behavior->SetPatrolMode();
		}
		
		return;
	}
	
	CheckNull(weapon);
	// Unarmed라면 Equipment 없음.
	if(weapon->GetEquipment() == nullptr)
	{
		behavior->SetEquipMode();
		return;
	}
	
	// 무기장착이 안되어 있다면 아래로 내려가지 말 것
	bool bEquipped = *(weapon->GetEquipment()->GetEquipped());
	CheckFalse(bEquipped);
	
	// 무기 장착
	float distance = ai->GetDistanceTo(target);
	if(distance < ActionRange)
	{
		behavior->SetActionMode();
		return;
	}

	bool result = team->TryRequestApproach(ai->GetOwner(), target);
	if(result)
	{
		behavior->SetApproachMode();
		return;
	}

	behavior->SetStrafeMode();
}
