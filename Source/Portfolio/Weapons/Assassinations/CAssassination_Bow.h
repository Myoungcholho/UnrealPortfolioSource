#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/Assassinations/CAssassination.h"
#include "CAssassination_Bow.generated.h"

UCLASS()
class PORTFOLIO_API UCAssassination_Bow : public UCAssassination
{
	GENERATED_BODY()

public:
	virtual void DoAssassination() override;

private:
	bool ExecuteBackstab();
};
