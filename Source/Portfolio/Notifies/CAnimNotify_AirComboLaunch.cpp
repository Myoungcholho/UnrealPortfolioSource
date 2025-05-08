#include "../Notifies/CAnimNotify_AirComboLaunch.h"
#include "../Global.h"

FString UCAnimNotify_AirComboLaunch::GetNotifyName_Implementation() const
{
	return "LaunchUpAirBorne";
}

void UCAnimNotify_AirComboLaunch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->LaunchCharacter(FVector(0,0,Launch),true,true);
	
}
