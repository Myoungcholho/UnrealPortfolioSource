#include "../Weapons/CAttachment_Hammer.h"
#include "../Global.h"

#include "GameFramework/Character.h"

ACAttachment_Hammer::ACAttachment_Hammer()
{
	FHelpers::CreateComponent<UStaticMeshComponent>(this,&StaticMeshClub,"StaticMesh",Root);
}

void ACAttachment_Hammer::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),InSocketName);
}

EWeaponType ACAttachment_Hammer::RetruenWeaponType()
{
	return EWeaponType::Club;
}
