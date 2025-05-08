#include "../Notifies/CAnimNotify_SetHitResponseType.h"
#include "../Global.h"

FString UCAnimNotify_SetHitResponseType::GetNotifyName_Implementation() const
{
	return "HitResponseChange";
}

void UCAnimNotify_SetHitResponseType::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp->GetOwner());
	
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);
	
	switch(Type)
	{
	case EHitResponseType::Normal:
		state->SetNormalMode();
		break;
	case EHitResponseType::SuperArmor:
		state->SetSuperArmorMode();
		break;
	case EHitResponseType::AntiAirborne:
		state->SetSetAntiAirborneMode();
		break;
	case EHitResponseType::Invincible:
		state->SetInvincibleMode();
		break;
	}
}
