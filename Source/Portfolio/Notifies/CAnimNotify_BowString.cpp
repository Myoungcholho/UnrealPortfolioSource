#include "../Notifies/CAnimNotify_BowString.h"
#include "../Global.h"
#include "../Components/CWeaponComponent.h"
#include "../Weapons/DoActions/CDoAction_Bow.h"

UCAnimNotify_BowString::UCAnimNotify_BowString()
{
#if WITH_EDITORONLY_DATA
	FLinearColor color = FLinearColor(0.3f, 1.0f, 0.5f);
	NotifyColor = color.ToFColor(true);
#endif
}

FString UCAnimNotify_BowString::GetNotifyName_Implementation() const
{
	return "BowString";
}

void UCAnimNotify_BowString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
	CheckNull(bow);

	if(LastAttack == false)
		bow->SetAttachedString();
	else
		bow->LastAttachedString();
}
