#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CStatusBase.generated.h"

UCLASS(Abstract)
class PORTFOLIO_API UCStatusBase : public UObject
{
	GENERATED_BODY()
public:
	FORCEINLINE float GetCurrent() { return Current;}
	FORCEINLINE float GetMax(){return Max;}
	
public:
	virtual void BeginPlay(class ACharacter* InOwner,float InCurrent, float InMax, float InRecoverySpeed, float InTickInterval = 0.1f);
	virtual void Tick();
	
public:
	virtual void UpdateStatus();
	virtual void Increase(float InAmount);
	virtual void Decrease(float InAmount);
	

protected:
	UPROPERTY()
	ACharacter* Owner;
	float Current;
	float Max;
	float RecoverySpeed;
	FTimerHandle TimerHandle;
};
