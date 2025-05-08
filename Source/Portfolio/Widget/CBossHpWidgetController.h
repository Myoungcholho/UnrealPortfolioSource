#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CBossHpWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossHealthChangedDelegate, float,InCurrentHealth,float,InMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossStunChangedDelegate, float, InCurrentStun, float, InMaxStun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossStatusWidgetToggle, bool, InActive);

UCLASS(BlueprintType)
class PORTFOLIO_API UCBossHpWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void RegisterBoss(class ACEnemy* InEnemy);
	void UnregisterBoss();

public:
	UFUNCTION(BlueprintCallable)
	float GetHealth();
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();
	UFUNCTION(BlueprintCallable)
	FString GetBossName();
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnBossStatusWidgetToggle OnBossStatusWidgetToggleDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBossHealthChangedDelegate OnBossHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnBossStunChangedDelegate OnBossStunChangedDelegate;

private:
	UFUNCTION()
	void BossHealthChangedDelegateCall(float InCurrentHealth, float InMaxHealth);
	UFUNCTION()
	void BossStunChangedDelegateCall(float InCurrentStun, float InMaxStun);
	
private:
	void ShowBossHPWidget();
	void HideBossHPWidget();
	
private:
	UPROPERTY()
	class ACEnemy* Boss;
};
