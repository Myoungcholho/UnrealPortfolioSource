#pragma once

#include "CoreMinimal.h"

class PORTFOLIO_API IStateComponentVisitor
{
public:
	virtual void Visit(class UCStateComponent* StateComp) = 0;
	virtual ~IStateComponentVisitor() = default;
};
