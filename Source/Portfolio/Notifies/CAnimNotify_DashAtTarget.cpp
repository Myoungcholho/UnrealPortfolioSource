#include "../Notifies/CAnimNotify_DashAtTarget.h"
#include "../Global.h"
#include "Portfolio/Components/CMoveComponent.h"

FString UCAnimNotify_DashAtTarget::GetNotifyName_Implementation() const
{
	return "DashToTarget";
}

void UCAnimNotify_DashAtTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCMoveComponent* movement = FHelpers::GetComponent<UCMoveComponent>(MeshComp->GetOwner());
	CheckNull(movement);

	switch(DashType)
	{
	case EDashType::Dest:
		movement->SetDestination();
		break;
	case EDashType::MoveTo:
		movement->MoveToTarget();
		break;
	}
}
