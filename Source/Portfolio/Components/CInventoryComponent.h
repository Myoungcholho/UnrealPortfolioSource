#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Portfolio/Items/CItemStructures.h"
#include "CInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const TArray<UCInventoryItem*>&, InventoryData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryToggle);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 MaxSlotSize = 24;

	int32 CurrSlotSize = 0;
	
public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	FOnInventoryUpdated OnInventoryUpdated;
	FOnInventoryToggle OnInventoryToggle;
	
public:
	void ToggleInventory();
	void AddItem(FItemData InData);
	void RemoveItem(class UCInventoryItem* InItem);
	
public:
	UFUNCTION()
	void MoveItem(int32 FromIndex, int32 ToIndex);
	
	UFUNCTION()
	void UseItemCall(int32 InIndex);

public:
	bool HasItemInSlot(int32 InventoryIndex);
	class UCInventoryItem* GetItemAtIndex(int32 InventoryIndex);
	void SetItemAtIndex(int32 InventoryIndex, class UCInventoryItem* QuickSlotItem);
	
public:
	void MoveItemQuickSlot(int32 InventoryIndex, int32 QuickSlotIndex);
	void SwapItemBetweenInventoryAndQuickSlot(int32 InventoryIndex, int32 QuickSlotIndex);
	
private:
	UPROPERTY()
	TArray<class UCInventoryItem*> ItemObjectDatas;
};
