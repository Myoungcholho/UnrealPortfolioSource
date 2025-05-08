#pragma once

#include "CoreMinimal.h"
#include "CWeaponStructures.h"
#include "GameFramework/Actor.h"
#include "Portfolio/Components/CStateComponent.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter* ,InAttacker,AActor*,InAttackerCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap,class ACharacter*, InAttacker, class ACharacter*, InOther);

UCLASS()
class PORTFOLIO_API ACAttachment : public AActor
{
	GENERATED_BODY()
private:
	friend class UCAnimNotifyState_Collision;
	
protected:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	class USceneComponent* Root;
	
public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginEquip();
	virtual void OnBeginEquip_Implementation(){}

	UFUNCTION(BlueprintNativeEvent)
	void OnUnEquip();
	virtual void OnUnEquip_Implementation(){}
	
protected:
	UFUNCTION(BlueprintCallable,Category="Attach")
	virtual void AttachTo(FName InSocketName);

	//Fist용 AttachCollision
	UFUNCTION(BlueprintCallable, Category ="Attach")
	void AttachToCollision(FName InCollisionName);

// 무기 충돌체 호출 바인딩 함수
private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 무기 콜리전 ON/OFF	
private:
	void OnCollision();
	void OffCollision();
	
public:
	// 충돌체 꺼지고 켜질때 BoardCast
	FAttachmentBeginCollision OnAttachmentBeginCollision;
	FAttachmentEndCollision OnAttachmentEndCollision;

	// 충돌되었고 끝났을때 BoardCast
	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap OnAttachmentEndOverlap;
	
	
////////////////////////////////////////////////////
protected:
	UPROPERTY(BlueprintReadOnly,Category="Game")
	class ACharacter* OwnerCharacter;

private:
	TArray<class UShapeComponent*> Collisions;

private:
	UFUNCTION()
	void OnDead(class AActor* DestroyedActor);
	
private:
	bool bIsTrackingCollision = false;
	void CreateCollisionTrace(UShapeComponent* ShapeComponent, float Duration);
	
	UPROPERTY()
	TArray<FVector> LocationCenterStack;
	UPROPERTY()
	TArray<FVector> LocationTopStack;
	
};
