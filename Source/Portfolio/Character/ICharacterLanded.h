#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacterLanded.generated.h"

// UObject를 상속하지 않아 멀티케스트 사용 불가능.
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLandedDelegate, const FHitResult&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLandedDelegate, const FHitResult&);

UINTERFACE(MinimalAPI)
class UICharacterLanded : public UInterface
{
	GENERATED_BODY()
};

class PORTFOLIO_API IICharacterLanded
{
	GENERATED_BODY()

public:
	virtual FOnLandedDelegate& GetOnLandedDelegate() = 0;
	
};
