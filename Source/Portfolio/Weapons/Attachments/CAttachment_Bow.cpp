#include "../../Weapons/Attachments/CAttachment_Bow.h"
#include "../../Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Portfolio/Weapons/Addons/CAnimInstance_Bow.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;

	FHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	FHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);

	USkeletalMesh* mesh;
	FHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Characters/Weapons/ElvenBow/SK_ElvenBow.SK_ElvenBow'");
	SkeletalMesh->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	FHelpers::GetClass<UCAnimInstance_Bow>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/Weapons/Bow/ABP_Bow.ABP_Bow_C'");
	SkeletalMesh->SetAnimInstanceClass(animInstance);
}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");

	SkeletalMesh->SetVisibility(false);

	USkeletalMesh* mesh = Cast<USkeletalMesh>(SkeletalMesh->GetSkinnedAsset());
	PoseableMesh->SetSkinnedAssetAndUpdate(mesh);
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
}

void ACAttachment_Bow::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Bow_LeftHand");
}

void ACAttachment_Bow::OnUnEquip_Implementation()
{
	Super::OnUnEquip_Implementation();

	AttachTo("Holster_Bow");
}

float* ACAttachment_Bow::GetAnimInstance_Bend()
{
	UCAnimInstance_Bow* bow = Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance());
	CheckNullResult(bow, nullptr);

	return bow->GetBend();
} 
