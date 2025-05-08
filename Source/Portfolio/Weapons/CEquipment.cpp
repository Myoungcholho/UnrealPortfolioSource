#include "../Weapons/CEquipment.h"
#include "../Global.h"

#include "../Components/CStateComponent.h"
#include "../Components/CMoveComponent.h"
#include "GameFramework/Character.h"

void UCEquipment::BeginPlay(class ACharacter* InCharacter, const FEquipmentData& InData)
{
	OwnerCharacter = InCharacter;
	Data = InData;

	State = FHelpers::GetComponent<UCStateComponent>(InCharacter);
	Movement = FHelpers::GetComponent<UCMoveComponent>(InCharacter);
}

// 무기 장착
void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if(Data.bCanMove == false)
		Movement->Stop();

	if(!!Data.EquipMontage)
	{
		OwnerCharacter->PlayAnimMontage(Data.EquipMontage,Data.EquipPlayRate);
		return;
	}

	// 몽타주 없는경우, 노티파이로 실행할 수 없으므로
	// 직접 실행
	Begin_Equip();
	End_Equip();
}

// 무기 장착 중 이벤트 발생 필요(어태치)
void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;
	
	if(OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();
}

// 무기 장착 중 이벤트 발생 필요(ex 장착 완료)
void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;

	if(Data.bCanMove == false)
		Movement->Move();

	State->SetIdleMode();
}

// 무기 해제
void UCEquipment::UnEquip_Implementation(bool InQuickUnEquip)
{
	State->SetEquipMode();

	if(Data.bCanMove == false)
		Movement->Stop();

	if(!!Data.UnEquipMontage && InQuickUnEquip == false)
	{
		OwnerCharacter->PlayAnimMontage(Data.UnEquipMontage,Data.UnEquipPlayRate);
		return;
	}
	
	Begin_UnEquip();
	End_UnEquip();
}

void UCEquipment::Begin_UnEquip_Implementation()
{
	if(OnEquipmentUnEquip.IsBound())
		OnEquipmentUnEquip.Broadcast();
}

void UCEquipment::End_UnEquip_Implementation()
{
	bEquipped = false;

	if(Data.bCanMove == false)
		Movement->Move();

	State->SetIdleMode();
}
