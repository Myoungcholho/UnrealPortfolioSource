#include "../Notifies/CAnimNotify_CameraBlend.h"
#include "../Global.h"
#include "Portfolio/Components/CCameraManagerComponent.h"

FString UCAnimNotify_CameraBlend::GetNotifyName_Implementation() const
{
	return "CameraBlend";
}

void UCAnimNotify_CameraBlend::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(BlendLocationName.IsNone())
		return;

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCCameraManagerComponent* manager = FHelpers::GetComponent<UCCameraManagerComponent>(MeshComp->GetOwner());
	CheckNull(manager);

	if(bZoom == true)
		manager->StartZoom();
	else
		manager->ChangeToSubCamera(BlendLocationName);

}
