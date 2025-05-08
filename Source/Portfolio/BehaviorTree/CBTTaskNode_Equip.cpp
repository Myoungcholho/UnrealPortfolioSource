#include "../BehaviorTree/CBTTaskNode_Equip.h"
#include "../Global.h"
#include "Portfolio/Components/CStateComponent.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"
#include "Portfolio/Weapons/CEquipment.h"


UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName ="Equip";
	bNotifyTick =true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACAIController* aiController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(aiController->GetPawn());

	UCWeaponComponent* Weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(Weapon,EBTNodeResult::Failed);
	CheckTrueResult(Type == EWeaponType::Max, EBTNodeResult::Failed);
	CheckTrueResult(Weapon->GetCurrentType() == Type , EBTNodeResult::Succeeded);

	switch (Type)
	{
		case EWeaponType::Katana: Weapon->SetKatanaMode(); break;
		case EWeaponType::Club: Weapon->SetClubMode(); break;
	}
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* aiController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(aiController->GetPawn());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	UCEquipment* equipment = weapon->GetEquipment();
	CheckNull(equipment);
	
	bool bEquipped = *(equipment->GetEquipped());
	
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);

	if(bEquipped && state->IsIdleMode())
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	UCEquipment* equipment = weapon->GetEquipment();

	//?? equipment가 왜 null이지 ? 그럴수가없는데
	// null이면 여기서 어떻게 처리해야하지?
	if(equipment == nullptr) return EBTNodeResult::Aborted;
	
	bool bBeginEquip = equipment->GetBeginEquip();

	if(bBeginEquip == false)
		weapon->GetEquipment()->Begin_Equip();

	weapon->GetEquipment()->End_Equip();

	return EBTNodeResult::Aborted;
}


