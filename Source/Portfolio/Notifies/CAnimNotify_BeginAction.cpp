#include "../Notifies/CAnimNotify_BeginAction.h"
#include "../Global.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Weapons/DoActions/CDoAction_Combo.h"


FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "BeginAction";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	
	// 공격 컴포넌트 가져오기
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	
	UCDoAction* doAction = weapon->GetDoAction();
	CheckNull(doAction);

	if(bAirBegin)
		doAction->Begin_Air_DoAction();
	else
		doAction->Begin_DoAction();
}
