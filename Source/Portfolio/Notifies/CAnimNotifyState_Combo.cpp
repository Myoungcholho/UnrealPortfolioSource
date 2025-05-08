#include "../Notifies/CAnimNotifyState_Combo.h"
#include "../Global.h"

#include "Portfolio/Character/CPlayer.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Weapons/DoActions/CDoAction_Bow.h"
#include "Portfolio/Weapons/DoActions/CDoAction_Combo.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	IIWeaponUser* weaponUser = Cast<IIWeaponUser>(owner);
	CheckNull(weaponUser);
	
	UCWeaponComponent* weapon = weaponUser->GetWeaponComponent();
	CheckNull(weapon);

	if(bBowCombo == true)
	{
		UCDoAction_Bow* doAction = Cast<UCDoAction_Bow>(weapon->GetDoAction());
		CheckNull(doAction);
		//FLog::Log("Bow EnableCombo");
		doAction->EnableCombo();
	}
	else
	{
		UCDoAction_Combo* doAction = Cast<UCDoAction_Combo>(weapon->GetDoAction());
		CheckNull(doAction);
		//FLog::Log("Katana EnableCombo");
		doAction->EnableCombo();
	}
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	
	ACharacter* owner = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(owner);

	IIWeaponUser* weaponUser = Cast<IIWeaponUser>(owner);
	CheckNull(weaponUser);
	
	UCWeaponComponent* weapon = weaponUser->GetWeaponComponent();
	CheckNull(weapon);

	if(bBowCombo == true)
	{
		UCDoAction_Bow* doAction = Cast<UCDoAction_Bow>(weapon->GetDoAction());
		CheckNull(doAction);
		//FLog::Log("Bow DisableCombo");
		doAction->DisableCombo();
	}
	else
	{
		UCDoAction_Combo* doAction = Cast<UCDoAction_Combo>(weapon->GetDoAction());
		CheckNull(doAction);
		//FLog::Log("Katana DisableCombo");
		doAction->DisableCombo();
	}
}
