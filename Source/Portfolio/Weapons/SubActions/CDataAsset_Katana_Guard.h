#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../CWeaponStructures.h"

#include "CDataAsset_Katana_Guard.generated.h"

UCLASS()
class PORTFOLIO_API UCDataAsset_Katana_Guard : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FGuardActivateData GuardActiveData;

	UPROPERTY(EditAnywhere)
	TArray<FGuardData> GuardDatas;

	UPROPERTY(EditAnywhere)
	FGuardBrokenData GuardBrokenData;
};
