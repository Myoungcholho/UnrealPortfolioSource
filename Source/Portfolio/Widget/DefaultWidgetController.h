#pragma once

#include "CoreMinimal.h"
#include "Portfolio/Items/CItemStructures.h"
#include "UObject/NoExportTypes.h"
#include "DefaultWidgetController.generated.h"


enum class EStateType : uint8;
enum class EWeaponType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChangedDelegate, EWeaponType, InPrevType, EWeaponType,InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChangedDelegate, EStateType, InPrevType, EStateType,InNewType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdatedDelegate, const TArray<UCInventoryItem*>&, InventoryData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryToggleDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotUpdatedDelegate, const TArray<UCInventoryItem*>&, QuickSlotData);
//--------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUseDelegate, int32, InSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemMoveDelegate, int32, PreviousSlotIndex, int32, TargetSlotIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimingToggleDelegate,bool,bToggle);

class UCStateComponent;
class UCWeaponComponent;
class UCInventoryComponent;
class UCQuickSlotComponent;
class UCStatusComponent;
class UCSubAction_Aiming;

class UCStatusWidgetController;
class UCBossHpWidgetController;
USTRUCT()
struct FPlayerParam
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UCStateComponent> State;
	UPROPERTY()
	TObjectPtr<UCWeaponComponent> Weapon;
	UPROPERTY()
	TObjectPtr<UCInventoryComponent> Inventory;
	UPROPERTY()
	TObjectPtr<UCQuickSlotComponent> QuickSlot;
	UPROPERTY()
	TObjectPtr<UCStatusComponent> Status;
};

UCLASS(Blueprintable,BlueprintType)
class PORTFOLIO_API UDefaultWidgetController : public UObject
{
	GENERATED_BODY()
public:
	void PreventGarbageCollection();
	void AllowGarbageCollection();

	
public:
	void InitWidgetController(FPlayerParam& Param);
	void BindWidgetDependency();

	UPROPERTY(BlueprintAssignable)
	FWeaponTypeChangedDelegate OnWeaponTypeChanged;

	UPROPERTY(BlueprintAssignable)
	FStateTypeChangedDelegate OnStateTypeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdatedDelegate OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryToggleDelegate OnInventoryToggle;

	UPROPERTY(BlueprintAssignable)
	FOnQuickSlotUpdatedDelegate OnQuickSlotUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnAimingToggleDelegate OnAimingToggle;
private:
	UFUNCTION()
	void WeaponTypeChangeDelegateCall(EWeaponType InPrevType, EWeaponType InNewType);

	UFUNCTION()
	void StateTypeChangeDelegateCall(EStateType InPrevType, EStateType InNewType);

public:
	// Item & Inventory
	UFUNCTION()
	void InventoryUpdateDelegateCall(const TArray<UCInventoryItem*>& InventoryData);
	UFUNCTION()
	void InventoryToggleDelegateCall();
	// QuickSlot
	UFUNCTION()
	void QuickSlotUpdateDelegateCall(const TArray<UCInventoryItem*>& QuickSlotData);
	// CrossHair
	UFUNCTION()
	void CrossHairToggleDelegateCall(bool bCrossHair);
public:
	// ✅ UI에서 인벤토리 정렬 버튼을 눌렀을 때 실행할 델리게이트
	FOnItemUseDelegate OnItemUseDelegate;
	FOnItemMoveDelegate OnItemMoveDelegate;
	
	
	UFUNCTION(BlueprintCallable)
	void RequestItemUse(int32 InIndex);

	UFUNCTION(BlueprintCallable)
	void RequestItemMove(int32 InPrev, int32 InTarget);
	
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCStateComponent> State;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCWeaponComponent> Weapon;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCInventoryComponent> Inventory;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCQuickSlotComponent> QuickSlot;
	
	// ---컨트롤러 분리 시작---
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCStatusWidgetController> StatusController;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCBossHpWidgetController> BossHpController;

private:
	bool bToggleOnOff = false;
};
