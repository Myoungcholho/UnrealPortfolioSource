#include "../Stamina/CStatusBase.h"
#include "../Global.h"

void UCStatusBase::BeginPlay(class ACharacter* InOwner, float InCurrent, float InMax,
	float InRecoverySpeed, float InTickInterval)
{
	Owner = InOwner;
	Current = InCurrent;
	Max = InMax;
	RecoverySpeed = InRecoverySpeed;
}

void UCStatusBase::Tick()
{
	//Tick
}

void UCStatusBase::UpdateStatus()
{
	// 타이머에 등록될 함수
}

void UCStatusBase::Increase(float InAmount)
{
	// 스테미너 회복	
}

void UCStatusBase::Decrease(float InAmount)
{
	// 스테미너 감소
}


