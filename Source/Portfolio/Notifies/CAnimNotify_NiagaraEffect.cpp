#include "../Notifies/CAnimNotify_NiagaraEffect.h"
#include "../Global.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

FString UCAnimNotify_NiagaraEffect::GetNotifyName_Implementation() const
{
	return "NiagaraEffect";
}

void UCAnimNotify_NiagaraEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(NiagaraEffect);
	
	if (SocketName == NAME_None)
		return;
	
	FVector Location = MeshComp->GetSocketLocation(SocketName);
	FRotator Rotation = MeshComp->GetSocketRotation(SocketName);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		MeshComp->GetWorld(),
		NiagaraEffect,
		Location,
		Rotation
	); 
}
