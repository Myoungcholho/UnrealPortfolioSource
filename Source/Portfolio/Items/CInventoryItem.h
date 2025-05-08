#pragma once

#include "CoreMinimal.h"
#include "CItemStructures.h"
#include "UObject/NoExportTypes.h"
#include "CInventoryItem.generated.h"

UCLASS(Blueprintable)
class PORTFOLIO_API UCInventoryItem : public UObject
{
	GENERATED_BODY()
public:
	virtual void Init(const FItemData& InItemData);

public:
	virtual bool CanUse(AActor* InActor){ return false;}
	virtual void Use(AActor* InActor){}
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item");
	FItemData ItemData;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item");
	int32 Quantity = 0;
};
