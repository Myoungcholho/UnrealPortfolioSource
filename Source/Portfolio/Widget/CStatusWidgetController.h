#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CStatusWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float , InCurrentHealth, float , InMaxHealth);

UCLASS()
class PORTFOLIO_API UCStatusWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void InitStatusController(class UCStatusComponent* InStatus);

public:
	UFUNCTION(BlueprintCallable)
	float GetHealth();
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChangedDelegate;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	class UCStatusComponent* Status;

private:
	void BindWidget();

private:
	UFUNCTION()
	void HealthChangedDelegateCall(float InCurrentHealth, float InMaxHealth);
};
