#include "../Stamina/CHealthStatus.h"
#include "../Global.h"

bool UCHealthStatus::IsDeadAfterDamage(float InAmount)
{
	if(Current-InAmount <= 0)
		return true;

	return false;
}

void UCHealthStatus::BeginPlay(class ACharacter* InOwner, float InCurrent, float InMax, float InRecoverySpeed,
	float InTickInterval)
{
	Super::BeginPlay(InOwner, InCurrent, InMax, InRecoverySpeed, InTickInterval);

	GetWorld()->GetTimerManager().SetTimer
	(
		TimerHandle,
		this,
		&UCHealthStatus::UpdateStatus,
		InTickInterval,
		true
	);
}

void UCHealthStatus::Decrease(float InAmount)
{
	Super::Decrease(InAmount);

	Current = Current - InAmount;
	if(Current <= 0)
		bDead = true;
	
	Current = FMath::Clamp(Current,0.0f,Max);
	
	if(OnHealthChanged.IsBound())
		OnHealthChanged.Broadcast(Current,Max);
}

void UCHealthStatus::Increase(float InAmount)
{
	Super::Increase(InAmount);

	Current = Current + InAmount;

	Current = FMath::Clamp(Current,0.0f,Max);

	if(OnHealthChanged.IsBound())
		OnHealthChanged.Broadcast(Current,Max);
}

void UCHealthStatus::UpdateStatus()
{
	Super::UpdateStatus();

	// timer 의해 0.1초마다 호출
	if(bDead == true)
		return;

	Current = Current + RecoverySpeed;
	Current = FMath::Clamp(Current,0.0f,Max);

	if(OnHealthChanged.IsBound())
		OnHealthChanged.Broadcast(Current,Max);
}
