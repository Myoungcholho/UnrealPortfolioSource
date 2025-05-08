#include "../../Weapons/Addons/CArrow.h"
#include "../../Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACArrow::ACArrow()
{
	FHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	FHelpers::CreateComponent<USkeletalMeshComponent>(this,&Mesh1,"Mesh",Capsule);
	FHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	// 스테틱 메시 설정
	USkeletalMesh* mesh;
	FHelpers::GetAsset(&mesh,"/Script/Engine.SkeletalMesh'/Game/Characters/Weapons/ElvenBow/SK_ElvenArrow1_SkelMesh.SK_ElvenArrow1_SkelMesh'");
	Mesh1->SetSkeletalMesh(mesh);
	Mesh1->SetCollisionProfileName(TEXT("NoCollision"));
	
	Capsule->SetCapsuleRadius(8);
	Capsule->SetCapsuleHalfHeight(8);

	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;
	Capsule->SetCollisionProfileName("BlockAll");


	Projectile->ProjectileGravityScale = 0.0f;
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);

	Projectile->SetActive(false);
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (OnArrowEndPlay.IsBound())
		OnArrowEndPlay.Broadcast(this);
}

void ACArrow::Init(class ACharacter* InOwner,class ACAttachment* InAttachment, FDoHitData& InData)
{
	SpawnOwner = InOwner;
	Attachment = InAttachment;
	hitData = InData;
}

void ACArrow::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->SetActive(true);

	SetLifeSpan(LifeSpanAfterSpawn);

	StartTrail();

	FTimerHandle handle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]()
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	});
	GetWorld()->GetTimerManager().SetTimer(handle, timerDelegate, 0.1f, false);
}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpanAfterCollision);
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	//if (!!character && OnArrowHit.IsBound())
		//OnArrowHit.Broadcast(this, character);
	CheckNull(character);

	AActor* attachment = Cast<AActor>(Attachment);
	CheckNull(Attachment);
	
	hitData.SendDamage(SpawnOwner,this,character);
}

EWeaponType ACArrow::RetruenWeaponType()
{
	return EWeaponType::Bow;
}
