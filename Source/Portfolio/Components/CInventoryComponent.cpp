#include "../Components/CInventoryComponent.h"

#include "CQuickSlotComponent.h"
#include "../Global.h"
#include "Portfolio/Items/CInventoryItem.h"
#include "Portfolio/Items/CInventoryItem_HealthPosition.h"
#include "Portfolio/Managers/CGameInstance.h"

UCInventoryComponent::UCInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemObjectDatas.SetNum(MaxSlotSize);
}

void UCInventoryComponent::ToggleInventory()
{
	//CheckNull(GetOwner());
	
	//FLog::Log("Inventory Toggle");
	if(OnInventoryUpdated.IsBound())
		OnInventoryUpdated.Broadcast(ItemObjectDatas);

	if(OnInventoryToggle.IsBound())
		OnInventoryToggle.Broadcast();
}

void UCInventoryComponent::AddItem(FItemData InData)
{
	// 1️⃣ 스택 가능한 동일한 아이템이 이미 존재하는지 확인
	for (UCInventoryItem* ExistingItem : ItemObjectDatas)
	{
		if (ExistingItem && ExistingItem->ItemData.Name == InData.Name)
		{
			// 2️⃣ 아이템이 스택 가능한 경우 , 1개씩 추가
			if (InData.bIsStackable && ExistingItem->Quantity + 1 <= InData.MaxStackSize)
			{
				ExistingItem->Quantity += 1;
				//UE_LOG(LogTemp, Warning, TEXT("InventoryComponent AddItem Called!"));
				if (OnInventoryUpdated.IsBound())
					OnInventoryUpdated.Broadcast(ItemObjectDatas);

				return;
			}
		}
	}

	if(InData.Type == EItemType::Consumable)
	{
		if(InData.ConsumableInfo.ConsumableType == EConsumableInfo::Health)
		{
			UCInventoryItem* NewItem = NewObject<UCInventoryItem>(this, UCInventoryItem_HealthPosition::StaticClass());
			CheckNull(NewItem);
			NewItem->Init(InData);

			//ItemObjectDatas.Add(NewItem);
			for (int32 i = 0; i < ItemObjectDatas.Num(); i++)
			{
				if (ItemObjectDatas[i] == nullptr) // 빈 슬롯 발견
				{
					ItemObjectDatas[i] = NewItem; // 빈 슬롯에 아이템 추가
					++CurrSlotSize;
					break;
				}
			}
		}
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("InventoryComponent AddItem Called!"));
	if(OnInventoryUpdated.IsBound())
		OnInventoryUpdated.Broadcast(ItemObjectDatas);
}

void UCInventoryComponent::RemoveItem(class UCInventoryItem* InItem)
{
	CheckNull(InItem);

	// 1️⃣ 아이템의 인덱스를 찾음
	int32 Index = ItemObjectDatas.Find(InItem);
	if (Index != INDEX_NONE)
	{
		ItemObjectDatas[Index] = nullptr;
	}

	InItem->ConditionalBeginDestroy();
}

// 드래그 이동 로직
void UCInventoryComponent::MoveItem(int32 FromIndex, int32 ToIndex)
{
	if(FromIndex == ToIndex)
		return;
	
	// 이동 주체 확인
	if(FromIndex < 0 || FromIndex >= MaxSlotSize)
		return;

	// 퀵슬롯의 이동이라면
	if(ToIndex==30 || ToIndex==31)
	{
		MoveItemQuickSlot(FromIndex,ToIndex);
		return;
	}

	UCInventoryItem* temp = ItemObjectDatas[FromIndex];
	ItemObjectDatas[FromIndex] = ItemObjectDatas[ToIndex];
	ItemObjectDatas[ToIndex] = temp;

	if(OnInventoryUpdated.IsBound())
		OnInventoryUpdated.Broadcast(ItemObjectDatas);
}

// DefaultWidgetController에 바인딩되어 호출됨.
void UCInventoryComponent::UseItemCall(int32 InIndex)
{
	AActor* owner = GetOwner();
	CheckNull(owner);

	if(ItemObjectDatas.Num() <= InIndex)
		return;

	//FLog::Log(ItemObjectDatas.Num());
	
	UCInventoryItem* item = ItemObjectDatas[InIndex];
	CheckNull(item);
	
	if(item->CanUse(owner) == false)
		return;

	item->Use(owner);

	if(item->Quantity <= 0)
		RemoveItem(item);

	if(OnInventoryUpdated.IsBound())
		OnInventoryUpdated.Broadcast(ItemObjectDatas);
}

bool UCInventoryComponent::HasItemInSlot(int32 InventoryIndex)
{
	if(ItemObjectDatas[InventoryIndex] == nullptr)
		return false;

	return true;
}

class UCInventoryItem* UCInventoryComponent::GetItemAtIndex(int32 InventoryIndex)
{
	return ItemObjectDatas[InventoryIndex];
}

void UCInventoryComponent::SetItemAtIndex(int32 InventoryIndex, class UCInventoryItem* QuickSlotItem)
{
	ItemObjectDatas[InventoryIndex] = QuickSlotItem;

	if(OnInventoryUpdated.IsBound())
		OnInventoryUpdated.Broadcast(ItemObjectDatas);
}

// [인벤토리 -> 퀵슬롯] 전송, 주체는 인벤토리
void UCInventoryComponent::MoveItemQuickSlot(int32 InventoryIndex, int32 QuickSlotIndex)
{
	AActor* Owner = GetOwner();
	CheckNull(Owner);

	UCQuickSlotComponent* QuickSlotComp = FHelpers::GetComponent<UCQuickSlotComponent>(Owner);
	CheckNull(QuickSlotComp);

	UCInventoryItem* MovingItem = ItemObjectDatas[InventoryIndex];
	CheckNull(MovingItem);

	// 퀵슬롯에 아이템이 있다면, 퀵슬롯 아이템을 내 위치로 이동
	if (QuickSlotComp->HasItemInSlot(QuickSlotIndex))
	{
		SwapItemBetweenInventoryAndQuickSlot(InventoryIndex, QuickSlotIndex);
	}
	else
	{
		// 퀵슬롯에 넘겨주고 난 참조해제
		QuickSlotComp->SetItemAtIndex(QuickSlotIndex,ItemObjectDatas[InventoryIndex]);
		ItemObjectDatas[InventoryIndex] = nullptr;
	}
	
	if(OnInventoryUpdated.IsBound())
		OnInventoryUpdated.Broadcast(ItemObjectDatas);
}

void UCInventoryComponent::SwapItemBetweenInventoryAndQuickSlot(int32 InventoryIndex, int32 QuickSlotIndex)
{
	AActor* Owner = GetOwner();
	CheckNull(Owner);

	UCQuickSlotComponent* QuickSlotComp = FHelpers::GetComponent<UCQuickSlotComponent>(Owner);
	CheckNull(QuickSlotComp);

	UCInventoryItem* InventoryItem = ItemObjectDatas[InventoryIndex];
	UCInventoryItem* QuickSlotItem = QuickSlotComp->GetItemAtIndex(QuickSlotIndex);

	// 두 아이템을 서로 교체
	ItemObjectDatas[InventoryIndex] = QuickSlotItem;
	QuickSlotComp->SetItemAtIndex(QuickSlotIndex, InventoryItem);
}
