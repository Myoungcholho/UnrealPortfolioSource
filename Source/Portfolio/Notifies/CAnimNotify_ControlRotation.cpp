#include "../Notifies/CAnimNotify_ControlRotation.h"
#include "../Global.h"
#include "Portfolio/Components/CMoveComponent.h"

FString UCAnimNotify_ControlRotation::GetNotifyName_Implementation() const
{
	return "ControlRotation";
}

void UCAnimNotify_ControlRotation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCMoveComponent* movement = FHelpers::GetComponent<UCMoveComponent>(MeshComp->GetOwner());
	CheckNull(movement);
	
	switch(type)
	{
	case EControlRotationMode::Lock:
		FLog::Log("Lock");
		movement->DisableControlRotation();	
		break;
	case EControlRotationMode::Unlock:
		FLog::Log("UnLock");
		movement->RequestEnableControlRotation();
		break;
	}
}
