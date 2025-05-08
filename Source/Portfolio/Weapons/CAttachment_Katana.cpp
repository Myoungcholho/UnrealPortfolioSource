#include "../Weapons/CAttachment_Katana.h"
#include "../Global.h"

#include "GameFramework/Character.h"

ACAttachment_Katana::ACAttachment_Katana()
{
	FHelpers::CreateComponent<UStaticMeshComponent>(this,&StaticMeshKatana,"StaticMesh",Root);
	
}

void ACAttachment_Katana::BeginPlay()
{
	Super::BeginPlay();
}

// 블루프린트에서 호출함
void ACAttachment_Katana::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),InSocketName);
	ScabbardSpawn();
}

EWeaponType ACAttachment_Katana::RetruenWeaponType()
{
	return EWeaponType::Katana;
}

void ACAttachment_Katana::ScabbardSpawn()
{
	const FString MeshPath = TEXT("/Script/Engine.StaticMesh'/Game/GhostSamurai_Bundle/GhostSamurai/Weapon/Mesh/Katana/SM_Scabbard01.SM_Scabbard01'");
    
	// Static Mesh 로드
	UStaticMesh* StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *MeshPath));
	if (!StaticMesh)
		return;

	// Static Mesh Component 생성
	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this);
	if (!MeshComponent)
		return;
	

	// 생성한 컴포넌트를 루트 컴포넌트에 부착
	MeshComponent->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,HolsterScabbard_SocketName );

	// Static Mesh 설정
	MeshComponent->SetStaticMesh(StaticMesh);

	// 생성한 컴포넌트를 활성화하고 월드에 등록
	MeshComponent->RegisterComponent();
	MeshComponent->SetWorldLocation(OwnerCharacter->GetMesh()->GetSocketLocation(HolsterScabbard_SocketName));

	// Static Mesh 설정
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetEnableGravity(false);
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	
}
