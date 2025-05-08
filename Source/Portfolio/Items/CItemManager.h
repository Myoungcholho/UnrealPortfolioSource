#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CItemStructures.h"
#include "NiagaraSystem.h"
#include "CItemManager.generated.h"

UCLASS()
class PORTFOLIO_API UCItemManager : public UObject
{
	GENERATED_BODY()
public:
	UCItemManager();

public:
	void ItemSpawn(FString InItemID, int32 InCount, FVector InLocation, FRotator InRotator); 
	
private:
	class UCItemRarityAsset* RarityAsset;

private:
	class UDataTable* ItemDataTable;

private:
	TArray<UNiagaraSystem*> VFXEffects;
	TArray<UNiagaraSystem*> TrailEffects;
	TArray<USoundBase*> Sounds;

private:
	TMap<FString,FItemData> DataMap;
};
