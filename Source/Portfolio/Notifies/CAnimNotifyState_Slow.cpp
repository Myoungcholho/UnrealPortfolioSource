#include "../Notifies/CAnimNotifyState_Slow.h"
#include "../Global.h"

FString UCAnimNotifyState_Slow::GetNotifyName_Implementation() const
{
	return "Slow";
}

void UCAnimNotifyState_Slow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	CheckNull(MeshComp);
	
	if (UAnimMontage* Montage = Cast<UAnimMontage>(Animation))
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->Montage_SetPlayRate(Montage, SlowRate); // 0.5배속으로 느려짐
		}
	}
}

void UCAnimNotifyState_Slow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	
	if (UAnimMontage* Montage = Cast<UAnimMontage>(Animation))
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->Montage_SetPlayRate(Montage, 1.0f); // 0.5배속으로 느려짐
		}
	}
}
