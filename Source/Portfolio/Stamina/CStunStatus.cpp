#include "../Stamina/CStunStatus.h"
#include "../Global.h"

void UCStunStatus::BeginPlay(class ACharacter* InOwner, float InCurrent, float InMax, float InRecoverySpeed,
	float InTickInterval)
{
	Super::BeginPlay(InOwner, InCurrent, InMax, InRecoverySpeed, InTickInterval);

	State = FHelpers::GetComponent<UCStateComponent>(InOwner);
	
	GetWorld()->GetTimerManager().SetTimer
	(
		TimerHandle,
		this,
		&UCStunStatus::UpdateStatus,
		InTickInterval,
		true
	);
}

void UCStunStatus::Decrease(float InAmount)
{
	Super::Decrease(InAmount);

	CheckNull(State);

	// 그로기 상태진입한다면 누적 불가능
	if(bStun)
		return;

	Current = Current + InAmount;

	// 그로기 시작
	if(Current >= Max)
	{
		State->AddStatusEffect(EStatusEffect::Weakened);
		bStun = true;
	}
	Current = FMath::Clamp(Current,0.0f,Max);

	if(OnStunChanged.IsBound())
		OnStunChanged.Broadcast(Current,Max);
}

void UCStunStatus::UpdateStatus()
{
	Super::UpdateStatus();

	CheckNull(State);
	if(bStun == false)
		return;
	
	Current = Current - RecoverySpeed;

	// 그로기 끝남
	if(Current <= 0)
	{
		State->RemoveStatusEffect(EStatusEffect::Weakened);
		bStun = false;
	}
	
	Current = FMath::Clamp(Current,0.0f,Max);

	if(OnStunChanged.IsBound())
		OnStunChanged.Broadcast(Current,Max);
}

void UCStunStatus::ResetStun()
{
	bStun = true;
	State->RemoveStatusEffect(EStatusEffect::Weakened);
	Current = 0.0f;
	
	if(OnStunChanged.IsBound())
		OnStunChanged.Broadcast(Current,Max);
}
