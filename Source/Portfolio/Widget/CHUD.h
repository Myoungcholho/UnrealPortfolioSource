#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

class UCHUD_Widget;
struct FPlayerParam;
class UCStateComponent;
class UCWeaponComponent;
class UCStateWidget;
class UDefaultWidgetController;
class UCInventoryComponent;
class UCQuickSlotComponent;
UCLASS()
class PORTFOLIO_API ACHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void InitOverlay(UCStateComponent* State,UCWeaponComponent* Weapon,
		UCInventoryComponent* Inventory, UCQuickSlotComponent* QuickSlot, UCStatusComponent* Status);

public:
	TObjectPtr<UDefaultWidgetController> GetDefaultWidgetController(){return DefaultWidgetController;}
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCHUD_Widget> HUDWidget;

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDefaultWidgetController> DefaultWidgetControllerClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDefaultWidgetController> DefaultWidgetController;
	
};
