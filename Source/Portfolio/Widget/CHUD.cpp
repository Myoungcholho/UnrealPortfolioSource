#include "../Widget/CHUD.h"
#include "../Global.h"

#include "CHUD_Widget.h"
#include "../Widget/DefaultWidgetController.h"
#include "Blueprint/UserWidget.h"
#include "Portfolio/Components/CInventoryComponent.h"
#include "Portfolio/Components/CQuickSlotComponent.h"
#include "Portfolio/Components/CStatusComponent.h"
#include "Portfolio/Weapons/SubActions/CSubAction_Aiming.h"

void ACHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(DefaultWidgetController))
	{
		DefaultWidgetController->AllowGarbageCollection(); // GC가 안전하게 삭제할 수 있도록 허용
		DefaultWidgetController = nullptr;
		UE_LOG(LogTemp, Log, TEXT("ACHUD : EndPlay - DefaultWidgetController Removed"));
	}
	
	Super::EndPlay(EndPlayReason);
}

void ACHUD::InitOverlay(UCStateComponent* State, UCWeaponComponent* Weapon,
	UCInventoryComponent* Inventory,UCQuickSlotComponent* QuickSlot, UCStatusComponent* Status)
{
	FPlayerParam playerParam;

	playerParam.State = State;
	playerParam.Weapon = Weapon;
	playerParam.Inventory = Inventory;
	playerParam.QuickSlot = QuickSlot;
	playerParam.Status = Status;
	
	DefaultWidgetController = NewObject<UDefaultWidgetController>(this,DefaultWidgetControllerClass);
	if(DefaultWidgetController != nullptr)
	{
		//DefaultWidgetController->AddToRoot(); // 자꾸 터져서 추가, 자동 정리하나..
		DefaultWidgetController->InitWidgetController(playerParam);
	}

	UUserWidget* userWidget = CreateWidget<UUserWidget>(GetWorld(),HUDWidgetClass);
	HUDWidget = Cast<UCHUD_Widget>(userWidget);

	HUDWidget->AddToViewport();
	HUDWidget->InitHudController(DefaultWidgetController);
	
}
