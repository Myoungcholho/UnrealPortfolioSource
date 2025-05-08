#include "../Notifies/CAnimNotify_CanCreateArrow.h"
#include "../Global.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Weapons/DoActions/CDoAction_Bow.h"

FString UCAnimNotify_CanCreateArrow::GetNotifyName_Implementation() const
{
	return "CanCreateArrow";
}

void UCAnimNotify_CanCreateArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	
	UCDoAction* doAction = weapon->GetDoAction();
	CheckNull(doAction);

	UCDoAction_Bow* bowDoAction = Cast<UCDoAction_Bow>(doAction);
	CheckNull(bowDoAction);

	bowDoAction->CanCreateArrow();
}
