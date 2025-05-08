#pragma once

#include "CoreMinimal.h"
#include "CStatusBase.h"
#include "UObject/NoExportTypes.h"
#include "CStunStatus.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStunChanged, float, InStun, float, InMaxStun);

UCLASS()
class PORTFOLIO_API UCStunStatus : public UCStatusBase
{
	GENERATED_BODY()
public:
	FORCEINLINE bool IsStun() { return bStun;}

public:
	FOnStunChanged OnStunChanged;

public:
	virtual void BeginPlay(class ACharacter* InOwner, float InCurrent, float InMax, float InRecoverySpeed, float InTickInterval = 0.1f) override;
	virtual void Decrease(float InAmount) override;
	virtual void UpdateStatus() override;
	
	void ResetStun();

private:
	bool bStun;
	
private:
	UPROPERTY()
	class UCStateComponent* State;
};
