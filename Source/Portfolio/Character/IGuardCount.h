#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGuardCount.generated.h"

UINTERFACE(MinimalAPI)
class UIGuardCount : public UInterface
{
	GENERATED_BODY()

	
};

class PORTFOLIO_API IIGuardCount
{
	GENERATED_BODY()
public:
	virtual void ReceiveGuardCounter(){}
};
