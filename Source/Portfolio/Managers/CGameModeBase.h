#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

UCLASS()
class PORTFOLIO_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	class UCHitEffectPostProcessController* GetPostProcessController() const { return PostProcessController; }
	
private:
	UPROPERTY()
	class UCHitEffectPostProcessController* PostProcessController;
};
