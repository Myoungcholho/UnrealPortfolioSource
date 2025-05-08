#pragma once

#include "CoreMinimal.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "UObject/Interface.h"
#include "IAssassination.generated.h"


UINTERFACE(MinimalAPI)
class UIAssassination : public UInterface
{
	GENERATED_BODY()
};
class PORTFOLIO_API IIAssassination
{
	GENERATED_BODY()
public:
	virtual bool CanBeAssassinated(EWeaponType InType,bool IsBack) = 0;
	virtual void ExecuteAssassination(ACharacter* Target,FExecutionData& InData, EWeaponType InType) {}
	virtual void OnAssassinated(ACharacter* Assassin, FExecutionData& InData, EWeaponType InType) {}
	virtual FExecutionMontageData* GetLastAssassinMontage(float InAmount){return nullptr;}
};
