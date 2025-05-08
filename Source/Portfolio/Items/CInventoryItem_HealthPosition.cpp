#include "../Items/CInventoryItem_HealthPosition.h"
#include "../Global.h"
#include "Portfolio/Components/CStatusComponent.h"
#include "Portfolio/Stamina/CHealthStatus.h"

void UCInventoryItem_HealthPosition::Init(const FItemData& InItemData)
{
	Super::Init(InItemData);

	HealAmount = InItemData.ConsumableInfo.ConsumablePower;
}

bool UCInventoryItem_HealthPosition::CanUse(AActor* InActor)
{
	UCStatusComponent* status = FHelpers::GetComponent<UCStatusComponent>(InActor);
	CheckNullResult(status,false);

	UCStatusBase* health = status->GetHealthStatus();
	CheckNullResult(health,false);

	bool bCheck = FMath::Abs(health->GetCurrent() - health->GetMax()) <= KINDA_SMALL_NUMBER;

	// 체력 꽉차있다면
	if(bCheck == true)
		return false;
	
	return true;
}

void UCInventoryItem_HealthPosition::Use(AActor* InActor)
{
	UCStatusComponent* status = FHelpers::GetComponent<UCStatusComponent>(InActor);
	CheckNull(status);

	UCHealthStatus* health = status->GetHealthStatus();
	CheckNull(health);

	float healAmount = ItemData.ConsumableInfo.ConsumablePower;
	health->Increase(healAmount);

	--Quantity;
}
