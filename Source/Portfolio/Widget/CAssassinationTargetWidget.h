#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CAssassinationTargetWidget.generated.h"

UCLASS()
class PORTFOLIO_API UCAssassinationTargetWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
public:
	UFUNCTION(BlueprintCallable)
	void OnTargetChanged(AActor* NewTarget);
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	AActor* TargetActor = nullptr;
};
