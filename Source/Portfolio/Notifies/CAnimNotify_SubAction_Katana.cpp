#include "../Notifies/CAnimNotify_SubAction_Katana.h"
#include "../Global.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Weapons/DoActions/CDoAction.h"
#include "Portfolio/Weapons/SubActions/CSubAction.h"

FString UCAnimNotify_SubAction_Katana::GetNotifyName_Implementation() const
{
	return "Katana_SubAction";
}

void UCAnimNotify_SubAction_Katana::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	UCSubAction* subAction = weapon->GetSubAction();
	CheckNull(subAction);

	UCDoAction* doAction = weapon->GetDoAction();
	CheckNull(doAction);
	
	switch(Type)
	{
	case EKatanaSubActionType::Begin_DrawGuard:
		{
			subAction->Begin_SubAction();
		}
		break;
	case EKatanaSubActionType::End_DrawGuard:
		{
			subAction->End_SubAction();
		}
		break;
	case EKatanaSubActionType::Begin_SheatheGuard:
		{
			subAction->Begin_UnSubAction();
		}
		break;
	case EKatanaSubActionType::End_SheatheGuard:
		{
			subAction->End_UnSubAction();
			doAction->End_DoAction();
		}
		break;
	}
}
