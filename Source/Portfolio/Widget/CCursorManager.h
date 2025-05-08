#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCursorManager.generated.h"

UCLASS()
class PORTFOLIO_API UCCursorManager : public UObject
{
	GENERATED_BODY()
	UCCursorManager();
public:
	UFUNCTION(BlueprintCallable)
	void ShowCursor();

	UFUNCTION(BlueprintCallable)
	void HideCursor();

private:
	int32 count;
};
