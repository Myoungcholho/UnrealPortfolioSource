#pragma once

#include "CoreMinimal.h"
#include "CStatusBase.h"
#include "UObject/NoExportTypes.h"
#include "CHealthStatus.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float , InCurrentHealth, float , InMaxHealth);

UCLASS()
class PORTFOLIO_API UCHealthStatus : public UCStatusBase
{
	GENERATED_BODY()
public:
	FORCEINLINE bool IsDead() { return Current<=0.0f;}

public:
	bool IsDeadAfterDamage(float InAmount);
	
public:
	FOnHealthChanged OnHealthChanged;

public:
	virtual void BeginPlay(class ACharacter* InOwner, float InCurrent, float InMax, float InRecoverySpeed, float InTickInterval=0.1f) override;
	virtual void Decrease(float InAmount) override;
	virtual void Increase(float InAmount) override;
	virtual void UpdateStatus() override;

private:
	bool bDead;
};
