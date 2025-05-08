#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IStatable.generated.h"

UINTERFACE(MinimalAPI)
class UIStatable : public UInterface
{
	GENERATED_BODY()
};

class PORTFOLIO_API IIStatable
{
	GENERATED_BODY()
public:
	virtual void End_Damaged(){}
	virtual void End_Dead(){}
	virtual void End_GuardImpact(){}
	virtual void End_Evade(){}
	virtual void End_Assassination(){}
};
