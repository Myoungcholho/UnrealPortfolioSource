#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Portfolio/Character/CEnemy.h"
#include "Portfolio/Enemies/CDataAsset_MonsterDefinitions.h"
#include "CHitMotionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCHitMotionComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere,Category="DataAsset")
	class UCHitMontageAsset* DataAsset;
	
	UPROPERTY(EditAnywhere,Category="DataAsset")
	FExecutionMontageData LastExecution;

public:
	FORCEINLINE FExecutionMontageData* GetLastExecution() { return &LastExecution; }
public:	
	UCHitMotionComponent();
	
protected:
	virtual void BeginPlay() override;

public:
	void HitReactionAssassinated(EWeaponType InWeaponType, EExecutionType InExecutionType);
	void HitReaction(ACharacter* InAttacker,FDoHitData* InHitData,EWeaponType InType,EMonsterType MonterType);
	void Dead();
	
	void SetDeadMontage(EDeadType InType,EWeaponType InWeaponType);
	void ReceiveGuardCounter();
	
private:
	UFUNCTION()
	void Landed(const FHitResult& Hit);

private:
	void NormalMonsterHit(FDoHitData* InHitData, class UCStateComponent* InState, EWeaponType InType);
	void BossMonsterHit(ACharacter* InAttacker,FDoHitData* InHitData, class UCStateComponent* InState, EWeaponType InType);
	
private:
	ACharacter* Owner;

// 죽는 타입, 암살 시 다른 Dead Motion용.
private:
	EDeadType DeadType = EDeadType::Max;
	EWeaponType DeadWeaponType = EWeaponType::Max;
};
