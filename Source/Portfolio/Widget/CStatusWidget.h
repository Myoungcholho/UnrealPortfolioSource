#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CStatusWidget.generated.h"

UCLASS()
class PORTFOLIO_API UCStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/*UFUNCTION(BlueprintCallable)
	void InitHudController(UDefaultWidgetController* Object);*/

	// 위에서 컨트롤러 초기화되면서 호출될 녀석, BeginPlay같은 거지
	/*UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void InitWidgetController();*/

protected:
	//UPROPERTY(BlueprintReadOnly)
	//TObjectPtr<UDefaultWidgetController> WidgetController;
	
};
