#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "CAssassinationManager.generated.h"

UCLASS(Blueprintable)
class PORTFOLIO_API UCAssassinationManager : public UObject
{
	GENERATED_BODY()
public:
	void TryAssassinate(ACharacter* Assassin, ACharacter* Target,FExecutionData& InData,EWeaponType InType);
};
