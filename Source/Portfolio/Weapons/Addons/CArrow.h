#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "Portfolio/Weapons/IWeaponType.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArrowEndPlay, class ACArrow*, InDestroyer);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArrowHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class PORTFOLIO_API ACArrow : public AActor
	,public IIWeaponType
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
	float LifeSpanAfterSpawn = 3;

	UPROPERTY(EditDefaultsOnly, Category="LifeSpan")
	float LifeSpanAfterCollision = 1;

private:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh1;
	
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* Projectile;
	
public:	
	ACArrow();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void Init(class ACharacter* InOwner, class ACAttachment* InAttachment, FDoHitData& InData);
	
	void Shoot(const FVector& InForward);

	UFUNCTION(BlueprintImplementableEvent)
	void StartTrail();
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
public:
	FArrowEndPlay OnArrowEndPlay;
	//FArrowHit OnArrowHit;

public:
	virtual EWeaponType RetruenWeaponType() override;
	
	
private:
	UPROPERTY()
	class ACharacter* SpawnOwner;
	UPROPERTY()
	class ACAttachment* Attachment;
	FDoHitData hitData;
	
};
