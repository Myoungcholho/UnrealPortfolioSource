#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"

UCLASS()
class PORTFOLIO_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,Category="Action")
	float ActionRange = 150;
	
	UPROPERTY(EditAnywhere,Category="Action")
	bool WaitTrue = false;

public:
	UCBTService_Melee();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
