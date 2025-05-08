#include "../Components/CQuickSlotComponent.h"

#include "CInventoryComponent.h"
#include "../Global.h"
#include "Portfolio/Items/CInventoryItem.h"

UCQuickSlotComponent::UCQuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	QuickItemDatas.SetNum(MaxSlotSize);
}

void UCQuickSlotComponent::PressQuickSlotZ()
{
	UseItem(0);
}

void UCQuickSlotComponent::PressQuickSlotX()
{
	UseItem(1);
}

void UCQuickSlotComponent::UseItem(int32 InIndex)
{
	AActor* owner = GetOwner();
	CheckNull(owner);
	
	UCInventoryItem* item = QuickItemDatas[InIndex];
	CheckNull(item);
	
	if(item->CanUse(owner) == false)
		return;

	item->Use(owner);

	// 수량 적으면 삭제
	if(item->Quantity <= 0)
	{
		int32 Index = QuickItemDatas.Find(item);
		if (Index != INDEX_NONE)
		{
			QuickItemDatas[Index] = nullptr;
		}

		item->ConditionalBeginDestroy();
	}

	if(OnQuickSlotUpdated.IsBound())
		OnQuickSlotUpdated.Broadcast(QuickItemDatas);
}

void UCQuickSlotComponent::MoveItem(int32 FromIndex, int32 ToIndex)
{
	if(FromIndex == ToIndex)
		return;
	
	// 이동 주체 확인
	if((FromIndex == 30 || FromIndex == 31) == false)
		return;

	// 인벤토리로의 이동이라면
	if(ToIndex >= 0 && ToIndex < 24)
	{
		MoveItemToInventory(FromIndex,ToIndex);
		return;
	}

	int32 MappingFrom = (FromIndex == 30) ? 0 : 1;
	int32 MappingTo = (ToIndex ==30) ? 0 : 1;
	
	UCInventoryItem* temp = QuickItemDatas[MappingFrom];
	QuickItemDatas[MappingFrom] = QuickItemDatas[MappingTo];
	QuickItemDatas[MappingTo] = temp;

	UpdateQuickSlotUI();
}

bool UCQuickSlotComponent::HasItemInSlot(int32 InQuickSlotIndex)
{
	if(InQuickSlotIndex == 30)
		return QuickItemDatas[0] != nullptr;

	if(InQuickSlotIndex == 31)
		return QuickItemDatas[1] != nullptr;

	return false;
}

class UCInventoryItem* UCQuickSlotComponent::GetItemAtIndex(int32 QuickSlotIndex)
{
	if(QuickSlotIndex == 30)
	{
		return QuickItemDatas[0];
	}
	if(QuickSlotIndex == 31)
	{
		return QuickItemDatas[1];
	}

	return nullptr;
}

void UCQuickSlotComponent::SetItemAtIndex(int32 QuickSlotIndex, UCInventoryItem* InventoryItem)
{
	if(QuickSlotIndex == 30)
	{
		QuickItemDatas[0] = InventoryItem;
	}
	if(QuickSlotIndex == 31)
	{
		QuickItemDatas[1] = InventoryItem;
	}

	UpdateQuickSlotUI();
}

void UCQuickSlotComponent::MoveItemToInventory(int32 QuickSlotIndex, int32 InventoryIndex)
{
	if((QuickSlotIndex == 30 || QuickSlotIndex == 31) == false)
		return;
	
	AActor* Owner = GetOwner();
	CheckNull(Owner);

	UCInventoryComponent* inventory = FHelpers::GetComponent<UCInventoryComponent>(Owner);
	CheckNull(inventory);

	if(QuickItemDatas.Num() > 2)
		return;

	int32 QuickItemIndex = (QuickSlotIndex == 30) ? 0 : 1;
	
	UCInventoryItem* MovingItem = QuickItemDatas[QuickItemIndex];
	CheckNull(MovingItem);

	// 인벤토리에 이미 아이템이 있으면 교환
	if (inventory->HasItemInSlot(InventoryIndex))
	{
		SwapItemBetweenQuickSlotAndInventory(QuickSlotIndex, InventoryIndex);
	}
	else
	{
		inventory->SetItemAtIndex(InventoryIndex, MovingItem);
		QuickItemDatas[QuickItemIndex] = nullptr;
	}

	UpdateQuickSlotUI();
}

void UCQuickSlotComponent::SwapItemBetweenQuickSlotAndInventory(int32 QuickSlotIndex, int32 InventoryIndex)
{
	AActor* Owner = GetOwner();
	CheckNull(Owner);

	UCInventoryComponent* InventoryComp = FHelpers::GetComponent<UCInventoryComponent>(Owner);
	CheckNull(InventoryComp);

	int32 QuickItemIndex = (QuickSlotIndex == 30) ? 0 : 1;
	
	UCInventoryItem* QuickSlotItem = QuickItemDatas[QuickItemIndex];
	UCInventoryItem* InventoryItem = InventoryComp->GetItemAtIndex(InventoryIndex);

	// 두 아이템을 서로 교체
	QuickItemDatas[QuickItemIndex] = InventoryItem;
	InventoryComp->SetItemAtIndex(InventoryIndex, QuickSlotItem);
}

void UCQuickSlotComponent::UpdateQuickSlotUI() const
{
	if(OnQuickSlotUpdated.IsBound())
		OnQuickSlotUpdated.Broadcast(QuickItemDatas);
}


