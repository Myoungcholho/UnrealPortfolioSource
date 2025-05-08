#include "../Widget/CBossHpWidgetController.h"
#include "../Global.h"
#include "Portfolio/Character/CEnemy.h"

#include "Portfolio/Components/CStatusComponent.h"
#include "Portfolio/Stamina/CHealthStatus.h"
#include "Portfolio/Stamina/CStunStatus.h"

void UCBossHpWidgetController::RegisterBoss(class ACEnemy* InEnemy)
{
	CheckNull(InEnemy);
	if(Boss == InEnemy) return;

	// 보스 있다면 언바인딩
	if(Boss)
	{
		UnregisterBoss();
	}

	Boss = InEnemy;

	UCStatusComponent* Status = FHelpers::GetComponent<UCStatusComponent>(Boss);
	CheckNull(Status);
	
	UCHealthStatus* health = Status->GetHealthStatus();
	CheckNull(health);
	health->OnHealthChanged.AddDynamic(this,&UCBossHpWidgetController::BossHealthChangedDelegateCall);

	UCStunStatus* Stun = Status->GetStunStatus();
	CheckNull(Stun);
	Stun->OnStunChanged.AddDynamic(this,&UCBossHpWidgetController::BossStunChangedDelegateCall);
	
	ShowBossHPWidget();
}

void UCBossHpWidgetController::UnregisterBoss()
{
	CheckNull(Boss);
	
	UCStatusComponent* Status = FHelpers::GetComponent<UCStatusComponent>(Boss);
	CheckNull(Status);
	
	UCHealthStatus* health = Status->GetHealthStatus();
	CheckNull(health);
	health->OnHealthChanged.RemoveDynamic(this,&UCBossHpWidgetController::BossHealthChangedDelegateCall);

	UCStunStatus* Stun = Status->GetStunStatus();
	CheckNull(Stun);
	Stun->OnStunChanged.RemoveDynamic(this,&UCBossHpWidgetController::BossStunChangedDelegateCall);

	HideBossHPWidget();
	
}

float UCBossHpWidgetController::GetHealth()
{
	CheckNullResult(Boss,0.0f);

	UCStatusComponent* Status = FHelpers::GetComponent<UCStatusComponent>(Boss);
	CheckNullResult(Status,0.0f);

	UCHealthStatus* health = Status->GetHealthStatus();
	CheckNullResult(health,0.0f);

	return health->GetCurrent();
}

float UCBossHpWidgetController::GetMaxHealth()
{
	CheckNullResult(Boss,0.0f);

	UCStatusComponent* Status = FHelpers::GetComponent<UCStatusComponent>(Boss);
	CheckNullResult(Status,0.0f);

	UCHealthStatus* health = Status->GetHealthStatus();
	CheckNullResult(health,0.0f);

	return health->GetMax();	
}

FString UCBossHpWidgetController::GetBossName()
{
	CheckNullResult(Boss,"");
	return Boss->GetName();
}

void UCBossHpWidgetController::BossHealthChangedDelegateCall(float InCurrentHealth, float InMaxHealth)
{
	if(OnBossHealthChangedDelegate.IsBound())
		OnBossHealthChangedDelegate.Broadcast(InCurrentHealth,InMaxHealth);
}

void UCBossHpWidgetController::BossStunChangedDelegateCall(float InCurrentStun, float InMaxStun)
{
	if(OnBossStunChangedDelegate.IsBound())
		OnBossStunChangedDelegate.Broadcast(InCurrentStun,InMaxStun);
}

void UCBossHpWidgetController::ShowBossHPWidget()
{
	if(OnBossStatusWidgetToggleDelegate.IsBound())
		OnBossStatusWidgetToggleDelegate.Broadcast(true);
}

void UCBossHpWidgetController::HideBossHPWidget()
{
	if(OnBossStatusWidgetToggleDelegate.IsBound())
		OnBossStatusWidgetToggleDelegate.Broadcast(false);
}
