// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ICharacterLanded.h"
#include "IGuardCount.h"
#include "IStatable.h"
#include "ITeam.h"
#include "GameFramework/Character.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "CEnemy.generated.h"

// 코드 중복있다. 해결하자
UENUM(BlueprintType)
enum class EDeadType : uint8
{
	Groggy,
	BackStab,
	LastExecution,
	Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnd_Dead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBegin_Dead);

enum class EStateType : uint8;

UCLASS()
class PORTFOLIO_API ACEnemy : public ACharacter ,
public IIStatable , public IIGuardCount, public IITeam, public IICharacterLanded
{
	GENERATED_BODY()
	friend ACAIController;
protected:
	UPROPERTY(VisibleAnywhere)
	class UCMoveComponent* Movement;

private:
	UPROPERTY(VisibleAnywhere)
	class UCLockonComponent* LockOn;
	
protected:
	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxTrigger;
	
protected:
	UPROPERTY(VisibleAnywhere)
	class UCStatusComponent* Stamina;

	UPROPERTY(VisibleAnywhere)
	class UCHitMotionComponent* HitMotion;

	UPROPERTY(VisibleAnywhere)
	class UCItemDropComponent* ItemDrop;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Team")
	uint8 TeamID = 2;

	UPROPERTY(EditDefaultsOnly, Category="MonsterDefinition")
	class UCDataAsset_MonsterDefinitions* MonsterData;

	UPROPERTY(EditDefaultsOnly, Category="Name")
	FString BossName ="Enemy";
	
////////////////////////////////////////////////
public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;
	
public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	FORCEINLINE FString GetName(){return BossName;}
	
private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
	void OnSubStateTypeChanged(ESubStateType InPrevType, ESubStateType InNewType);
	
protected:
	virtual void Damaged();
	
	
	virtual void HitLaunch(FDoHitData* InHitData){}
	virtual void Landed(const FHitResult& Hit) override;

	// 폭발형인지 아닌지 구별
	FVector CalculateLaunchDirection(const FVector& Start, const FVector& Target,const FVector& AttackerForward, bool bIsExplosive);
	// Launch 적용
	void ApplyLaunchForce(const FVector& Direction, float XYLaunch, float ZLaunch);
	// 피격자는 공격자를 바라볼 수 있도록
	void SetTargetRotation(const FVector& Start, const FVector& Target);
	
protected:
	virtual void Dead();
	virtual void Idle(){}
	
///////-Interface-///////
public:
	virtual void End_Dead() override;
	virtual void End_Damaged() override;
	virtual void End_GuardImpact() override;	// 가드 깨지고 나면 호출
	virtual void End_DoAction(){}
public:
	virtual void ReceiveGuardCounter() override;

public:
	virtual void SetTeamID(uint8 InTeamID) override;
	virtual uint8 GetTeamID() override;
///////-Interface-///////
// Dead상태를 알리는 Delegate
public:
	FBegin_Dead OnBegin_Dead;		// 죽으면 ClearTarget()용
	FEnd_Dead OnEnd_Dead;			// 실제 사용하는 곳은 없음
	FOnLandedDelegate OnLandedDelegate;
public:
	virtual FOnLandedDelegate& GetOnLandedDelegate() override { return OnLandedDelegate; }
	
protected:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	};
	FDamageData DamageData;

// Enemy Death Data
private:
	EDeadType DeadType = EDeadType::Max;
};