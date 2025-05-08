#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeaponUser.generated.h"

UINTERFACE(MinimalAPI)
class UIWeaponUser : public UInterface
{
	GENERATED_BODY()
};

class PORTFOLIO_API IIWeaponUser
{
	GENERATED_BODY()
public:
	virtual class UCWeaponComponent* GetWeaponComponent()=0;
	
};
