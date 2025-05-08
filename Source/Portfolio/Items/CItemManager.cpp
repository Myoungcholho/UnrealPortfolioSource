#include "../Items/CItemManager.h"
#include "../Global.h"

#include "CItem.h"
#include "CItemRarityAsset.h"
#include "CItemStructures.h"

UCItemManager::UCItemManager()
{
	FHelpers::GetAsset<UCItemRarityAsset>(&RarityAsset,"/Script/Portfolio.CItemRarityAsset'/Game/Item/DA_Ratiry.DA_Ratiry'");
	CheckNull(RarityAsset);
	
	FHelpers::GetAsset<UDataTable>(&ItemDataTable,"/Script/Engine.DataTable'/Game/Item/DT_DataTable.DT_DataTable'");
	CheckNull(ItemDataTable);
	
	TArray<UNiagaraSystem*> vfxDatas =  RarityAsset->GetVFXEffects();
	TArray<UNiagaraSystem*> trailDatas =  RarityAsset->GetTrailEffects();
	TArray<USoundBase*> sounds =  RarityAsset->GetSounds();
	
	for(int32 i=0; i<vfxDatas.Num(); ++i)
	{
		VFXEffects.Add(vfxDatas[i]);
		TrailEffects.Add(trailDatas[i]);
		Sounds.Add(sounds[i]);
	}

	//DataTable
	TArray<FItemData*> rows;
	ItemDataTable->GetAllRows<FItemData>("", rows);
	
	for (FItemData* data : rows)
	{
		DataMap.Add(data->Name,*data);
	}
}

void UCItemManager::ItemSpawn(FString InItemID, int32 InCount, FVector InLocation, FRotator InRotator)
{
	FItemData data = DataMap[InItemID];

	UWorld* World = GetWorld();
	CheckNull(World);

	
	UClass* BPItemClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Item/BP_CItem.BP_CItem_C'"));
	CheckNull(BPItemClass);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	ACItem* spawnedItem = World->SpawnActor<ACItem>(BPItemClass, InLocation, InRotator, SpawnParams);
	CheckNull(spawnedItem);

	spawnedItem->SetItemInfo(data,VFXEffects[(int32)data.Rarity],TrailEffects[(int32)data.Rarity],Sounds[(int32)data.Rarity]);
}
