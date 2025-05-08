#include "../Widget/CStatusWidgetController.h"
#include "../Global.h"

#include "Portfolio/Components/CStatusComponent.h"
#include "Portfolio/Stamina/CHealthStatus.h"


void UCStatusWidgetController::InitStatusController(class UCStatusComponent* InStatus)
{
	Status = InStatus;

	BindWidget();
}

float UCStatusWidgetController::GetHealth()
{
	if(!!Status)
	{
		UCHealthStatus* health = Status->GetHealthStatus();
		CheckNullResult(health,0.0f);

		return health->GetCurrent();
	}
	return 0.0f;
}

float UCStatusWidgetController::GetMaxHealth()
{
	if(!!Status)
	{
		UCHealthStatus* health = Status->GetHealthStatus();
		CheckNullResult(health,0.0f);

		return health->GetMax();
	}
	return 0.0f;
}

void UCStatusWidgetController::BindWidget()
{
	if(!!Status)
	{
		UCHealthStatus* health = Status->GetHealthStatus();
		CheckNull(health);

		health->OnHealthChanged.AddDynamic(this,&UCStatusWidgetController::HealthChangedDelegateCall);
		
	}
}

void UCStatusWidgetController::HealthChangedDelegateCall(float InCurrentHealth, float InMaxHealth)
{
	if(OnHealthChangedDelegate.IsBound())
		OnHealthChangedDelegate.Broadcast(InCurrentHealth,InMaxHealth);
}
