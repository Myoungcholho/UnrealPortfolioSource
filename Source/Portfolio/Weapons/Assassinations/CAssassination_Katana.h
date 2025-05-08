#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/Assassinations/CAssassination.h"
#include "CAssassination_Katana.generated.h"

UCLASS()
class PORTFOLIO_API UCAssassination_Katana : public UCAssassination
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(class ACharacter* InOwner, const TArray<FExecutionData>& InDatas) override;
	
public:
	virtual void DoAssassination() override;

private:
	bool ExecuteGroggyTarget();
	bool ExecuteBackstab();
};