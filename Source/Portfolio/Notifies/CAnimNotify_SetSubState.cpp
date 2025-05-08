#include "../Notifies/CAnimNotify_SetSubState.h"
#include "../Global.h"
#include "Portfolio/Components/CStateComponent.h"

FString UCAnimNotify_SetSubState::GetNotifyName_Implementation() const
{
	return "SetSubState";
}

void UCAnimNotify_SetSubState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	
	AActor* owner = MeshComp->GetOwner();
	CheckNull(owner);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(owner);
	CheckNull(state);

	switch(StateType)
	{
	case ESubStateType::None:
		state->SetSubNoneMode();
		break;
		
	case ESubStateType::Down:
		state->SetSubDownMode();
		break;

	case ESubStateType::Airborne:
		state->SetSubAirborneMode();
		break;

	default:
		break;
	}
}
