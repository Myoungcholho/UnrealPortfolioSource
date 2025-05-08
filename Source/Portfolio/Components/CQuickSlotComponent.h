#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CQuickSlotComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotUpdated, const TArray<UCInventoryItem*>&, QuickSlotData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	int32 MaxSlotSize = 2;
	
public:	
	UCQuickSlotComponent();

protected:
	virtual void BeginPlay() override;
public:
	FOnQuickSlotUpdated OnQuickSlotUpdated;

// Player Input
public:
	void PressQuickSlotZ();
	void PressQuickSlotX();
private:
	void UseItem(int32 InIndex);
	
	
//Controller에서 연결되어 호출당함
public:
	UFUNCTION()
	void MoveItem(int32 FromIndex, int32 ToIndex);
	
// Inventory Quick 교환
public:
	bool HasItemInSlot(int32 InQuickSlotIndex);
	class UCInventoryItem* GetItemAtIndex(int32 QuickSlotIndex);
	void SetItemAtIndex(int32 QuickSlotIndex, UCInventoryItem* InventoryItem);
public:
	void MoveItemToInventory(int32 QuickSlotIndex, int32 InventoryIndex);

private:
	void SwapItemBetweenQuickSlotAndInventory(int32 QuickSlotIndex, int32 InventoryIndex);
	void UpdateQuickSlotUI() const;
	
private:
	UPROPERTY()
	TArray<class UCInventoryItem*> QuickItemDatas;
};
