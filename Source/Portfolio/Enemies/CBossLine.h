#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Portfolio/Character/CEnemy.h"
#include "CBossLine.generated.h"


UCLASS()
class PORTFOLIO_API ACBossLine : public ATriggerBox
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	class ACEnemy* Boss;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
