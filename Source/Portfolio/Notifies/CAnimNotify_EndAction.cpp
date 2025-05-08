#include "../Notifies/CAnimNotify_EndAction.h"
#include "../Global.h"
#include "Portfolio/Character/CEnemy.h"

#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Weapons/DoActions/CDoAction.h"


FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	
	AActor* owner = MeshComp->GetOwner();
	CheckNull(owner);
	
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	UCDoAction* doAction = weapon->GetDoAction();
	CheckNull(doAction);

	doAction->End_DoAction();

	ACEnemy* enemy = Cast<ACEnemy>(owner);
	if(enemy != nullptr)
		enemy->End_DoAction();
}
