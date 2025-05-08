#include "../Notifies/CAnimNotify_ShootArrow.h"
#include "../Global.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Weapons/DoActions/CDoAction_Bow.h"

FString UCAnimNotify_ShootArrow::GetNotifyName_Implementation() const
{
	return "ShootArrow";
}

void UCAnimNotify_ShootArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	
	UCDoAction* doAction = weapon->GetDoAction();
	CheckNull(doAction);

	UCDoAction_Bow* bowDoAction = Cast<UCDoAction_Bow>(doAction);
	CheckNull(bowDoAction);

	bowDoAction->ShootArrow();
}
