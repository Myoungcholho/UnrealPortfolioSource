#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHUD_Widget.generated.h"

class UDefaultWidgetController;

UCLASS(Blueprintable,BlueprintType)
class PORTFOLIO_API UCHUD_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitHudController(UDefaultWidgetController* Object);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void InitWidgetController();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDefaultWidgetController> WidgetController;
};
