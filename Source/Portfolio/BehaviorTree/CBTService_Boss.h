#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Boss.generated.h"

UCLASS()
class PORTFOLIO_API UCBTService_Boss : public UBTService
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,Category="Action")
	FVector2D ActionInterval;
	
public:
	UCBTService_Boss();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CanProvideService(UBehaviorTreeComponent& OwnerComp);

private:
	float ActionCooldown;
};
