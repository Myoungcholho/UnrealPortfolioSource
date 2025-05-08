#include "../Widget/DefaultWidgetController.h"

#include "CBossHpWidgetController.h"
#include "CCursorManager.h"
#include "CStatusWidgetController.h"
#include "../Global.h"

#include "Portfolio/Components/CDetectionComponent.h"
#include "Portfolio/Components/CInventoryComponent.h"
#include "Portfolio/Components/CQuickSlotComponent.h"
#include "Portfolio/Components/CStateComponent.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Managers/CGameInstance.h"
#include "Portfolio/Weapons/SubActions/CSubAction_Aiming.h"

// 사실 필욘없는데 냅두자
void UDefaultWidgetController::PreventGarbageCollection()
{
	if (!IsRooted())
		AddToRoot();
}

void UDefaultWidgetController::AllowGarbageCollection()
{
	if (IsRooted())
		RemoveFromRoot();
}

void UDefaultWidgetController::InitWidgetController(FPlayerParam& Param)
{
	State = Param.State;
	Weapon = Param.Weapon;
	Inventory = Param.Inventory;
	QuickSlot = Param.QuickSlot;

	// 컨트롤러 생성
	StatusController = NewObject<UCStatusWidgetController>(this);
	StatusController->InitStatusController(Param.Status);

	BossHpController = NewObject<UCBossHpWidgetController>(this);
	
	BindWidgetDependency();

	// GC
	PreventGarbageCollection();
}

void UDefaultWidgetController::BindWidgetDependency()
{
	if(!!Weapon)
	{
		Weapon->OnWeaponTypeChanged.AddDynamic(this,&UDefaultWidgetController::WeaponTypeChangeDelegateCall);
	}
	if(!!State)
	{
		State->OnStateTypeChanged.AddDynamic(this,&UDefaultWidgetController::StateTypeChangeDelegateCall);
	}
	if(!!Inventory)
	{
		Inventory->OnInventoryUpdated.AddDynamic(this,&UDefaultWidgetController::InventoryUpdateDelegateCall);
		Inventory->OnInventoryToggle.AddDynamic(this,&UDefaultWidgetController::InventoryToggleDelegateCall);
		//FLog::Log("BindWidgetDependency");
		
		OnItemUseDelegate.AddDynamic(Inventory,&UCInventoryComponent::UseItemCall);
		OnItemMoveDelegate.AddDynamic(Inventory,&UCInventoryComponent::MoveItem);
	}
	if(!!QuickSlot)
	{
		QuickSlot->OnQuickSlotUpdated.AddDynamic(this,&UDefaultWidgetController::QuickSlotUpdateDelegateCall);
		OnItemMoveDelegate.AddDynamic(QuickSlot,&UCQuickSlotComponent::MoveItem);
	}
}

void UDefaultWidgetController::WeaponTypeChangeDelegateCall(EWeaponType InPrevType, EWeaponType InNewType)
{
	if(OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(InPrevType, InNewType);

	// Bow일때 바인딩 & 해제
	if(InPrevType == EWeaponType::Bow)
	{
		if(!!Weapon)
		{
			if(UCSubAction_Aiming* prevSubAction = Cast<UCSubAction_Aiming>(Weapon->GetPrevSubAction()))
			{
				//FLog::Log("UnBind Aiming");
				prevSubAction->OnAimingToggle.RemoveDynamic(this,&UDefaultWidgetController::CrossHairToggleDelegateCall);
			}
		}
	}

	if(InNewType == EWeaponType::Bow)
	{
		if(!!Weapon)
		{
			if(UCSubAction_Aiming* bowSubAction = Cast<UCSubAction_Aiming>(Weapon->GetSubAction()))
			{
				//FLog::Log("Bind Aiming");
				bowSubAction->OnAimingToggle.AddDynamic(this,&UDefaultWidgetController::CrossHairToggleDelegateCall);
			}
		}
	}
	
}

void UDefaultWidgetController::StateTypeChangeDelegateCall(EStateType InPrevType, EStateType InNewType)
{
	if(OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(InPrevType,InNewType);
}

void UDefaultWidgetController::InventoryUpdateDelegateCall(const TArray<UCInventoryItem*>& InventoryData)
{
	//UE_LOG(LogTemp, Warning, TEXT("DefaultWidget InventoryUpdateDelegateCall Called!"));
	if(OnInventoryUpdated.IsBound())
		OnInventoryUpdated.Broadcast(InventoryData);
}

void UDefaultWidgetController::InventoryToggleDelegateCall()
{
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNull(instance);
	UCCursorManager* manager = instance->GetCursorManager();
	CheckNull(manager);
	
	// On/Off 처리
	bToggleOnOff = !bToggleOnOff;
	
	if(bToggleOnOff)
		manager->ShowCursor();
	else
		manager->HideCursor();
	
	if(OnInventoryToggle.IsBound())
		OnInventoryToggle.Broadcast();
}

void UDefaultWidgetController::QuickSlotUpdateDelegateCall(const TArray<UCInventoryItem*>& QuickSlotData)
{
	if(OnQuickSlotUpdated.IsBound())
		OnQuickSlotUpdated.Broadcast(QuickSlotData);
}

void UDefaultWidgetController::CrossHairToggleDelegateCall(bool bCrossHair)
{
	if(OnAimingToggle.IsBound())
		OnAimingToggle.Broadcast(bCrossHair);
}

// 버튼이 눌렸을 때 BP에서 실행
void UDefaultWidgetController::RequestItemUse(int32 InIndex)
{
	if (OnItemUseDelegate.IsBound())
		OnItemUseDelegate.Broadcast(InIndex);
	
}

// BP에서 호출해 아이템 배열의 순위를 바꿈
void UDefaultWidgetController::RequestItemMove(int32 InPrev, int32 InTarget)
{
	if(OnItemMoveDelegate.IsBound())
		OnItemMoveDelegate.Broadcast(InPrev,InTarget);
}
