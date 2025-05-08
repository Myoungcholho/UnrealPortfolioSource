#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CEnemyWidget.generated.h"

UCLASS()
class PORTFOLIO_API UCEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//OnHealthPointChanged
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Widget")
	void UpdateHealth(float InCurrentHealth,float InMaxHealth);
	//OnStateTypeChanged
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Widget")
	void UpdateState(EStateType InPrevType,EStateType InNewType);
	//OnAIStateTypeChanged
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Widget")
	void UpdateAIState(EAIStateType InPrevType,EAIStateType InNewType);
	//Name
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Widget")
	void UpdateName(FName InName);
	//Stun
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Widget")
	void UpdateStun(float InCurrentStun, float InMaxStun);
	
};
