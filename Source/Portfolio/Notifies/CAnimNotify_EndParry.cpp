#include "../Notifies/CAnimNotify_EndParry.h"
#include "../Global.h"
#include "GameFramework/Character.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Weapons/SubActions/CSubAction_Katana.h"

FString UCAnimNotify_EndParry::GetNotifyName_Implementation() const
{
	return "End_Parry";
}

void UCAnimNotify_EndParry::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);

	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(owner);
	CheckNull(weapon);

	UCSubAction_Katana* subAction = Cast<UCSubAction_Katana>(weapon->GetSubAction());
	CheckNull(subAction);

	switch(GuardType)
	{
	case EGuardType::Parry:
		subAction->End_Parry();
		break;
	case EGuardType::Broken:
		subAction->End_Broken();
		break;
	}
}
