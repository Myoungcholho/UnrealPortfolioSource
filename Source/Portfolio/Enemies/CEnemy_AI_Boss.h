#pragma once

#include "CoreMinimal.h"
#include "CEnemy_AI.h"
#include "Portfolio/Assassination/IAssassination.h"
#include "CEnemy_AI_Boss.generated.h"

UCLASS()
class PORTFOLIO_API ACEnemy_AI_Boss : public ACEnemy_AI
	,public IIAssassination
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category="Launch")
	float BossKnockbackScale = 2.0f;

public:
	FORCEINLINE void LockRotation() { bRotationLocked = true; }
	FORCEINLINE void UnlockRotation() { bRotationLocked = false; } 
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	virtual void HitLaunch(FDoHitData* InHitData) override;

protected:
	virtual void End_DoAction() override;

protected:
	virtual bool CanBeAssassinated(EWeaponType InType,bool IsBack) override;
	virtual void OnAssassinated(ACharacter* Assassin, FExecutionData& InData, EWeaponType InType) override;
	virtual FExecutionMontageData* GetLastAssassinMontage(float InAmount) override;
	
protected:
	virtual void ReceiveGuardCounter() override;
private:
	bool bRotationLocked = false;
	
};
