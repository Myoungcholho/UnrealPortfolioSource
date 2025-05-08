#pragma once

#include "CoreMinimal.h"
#include "../Items/CInventoryItem.h"
#include "CInventoryItem_HealthPosition.generated.h"

UCLASS()
class PORTFOLIO_API UCInventoryItem_HealthPosition : public UCInventoryItem
{
	GENERATED_BODY()
public:
	float HealAmount = 0.0f;
	
	virtual void Init(const FItemData& InItemData) override;

public:
	virtual bool CanUse(AActor* InActor) override;
	virtual void Use(AActor* InActor) override;
};
