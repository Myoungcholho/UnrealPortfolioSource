#pragma once

#include "CoreMinimal.h"
#include "CWeaponStructures.h"
#include "UObject/Interface.h"
#include "IWeaponType.generated.h"

UINTERFACE(MinimalAPI)
class UIWeaponType : public UInterface
{
	GENERATED_BODY()
};

class PORTFOLIO_API IIWeaponType
{
	GENERATED_BODY()
public:
	virtual EWeaponType RetruenWeaponType()=0;
};
