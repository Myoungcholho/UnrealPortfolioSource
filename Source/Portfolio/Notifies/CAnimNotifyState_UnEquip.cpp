#include "../Notifies/CAnimNotifyState_UnEquip.h"

#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Utilities/FHelpers.h"
#include "Portfolio/Utilities/FLog.h"
#include "Portfolio/Weapons/CEquipment.h"

FString UCAnimNotifyState_UnEquip::GetNotifyName_Implementation() const
{
	return "UnEquip";
}

void UCAnimNotifyState_UnEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	// 상태가 바뀐 상태로 얻어와야 해서 일단 이렇게 해봄
	UCEquipment* equipment = weapon->GetPrevEquipment();
	CheckNull(equipment);

	equipment->Begin_UnEquip();
}

void UCAnimNotifyState_UnEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	UCEquipment* equipment = weapon->GetPrevEquipment();
	CheckNull(equipment);

	equipment->End_UnEquip();
}
