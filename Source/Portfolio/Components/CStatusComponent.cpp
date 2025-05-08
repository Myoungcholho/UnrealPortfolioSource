#include "../Components/CStatusComponent.h"
#include "../Global.h"

#include "GameFramework/Character.h"
#include "Portfolio/Stamina/CHealthStatus.h"
#include "Portfolio/Stamina/CStaminaAsset.h"
#include "Portfolio/Stamina/CStunStatus.h"

UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	CheckNull(owner);
	
	constexpr FHealthData DefaultHealthData = { 100.0f, 100.0f, 5.0f };
	constexpr FStunData DefaultStunData = { 0.0f, 100.0f, 5.0f };
	
	FHealthData HealthData = (StaminaAsset) ? StaminaAsset->GetHealthData() : DefaultHealthData;
	FStunData StunData = (StaminaAsset) ? StaminaAsset->GetStunData() : DefaultStunData;
	
	// Health 초기화
	if(Health == nullptr)
	{
		Health = NewObject<UCHealthStatus>(this);
		if(Health)
		{
			FHealthData Data = StaminaAsset->GetHealthData(); 
			Health->BeginPlay(owner,HealthData.Health,HealthData.MaxHealth,HealthData.RecoveryHealth);
			StatusList.Add(Health);
		}
	}

	// Stun 초기화
	if(Stun == nullptr)
	{
		Stun = NewObject<UCStunStatus>(this);
		if(Stun)
		{
			FStunData Data = StaminaAsset->GetStunData();
			Stun->BeginPlay(owner,StunData.Stun,StunData.MaxStun,StunData.RecoveryStun);
			StatusList.Add(Stun);
		}
	}
	
}

void UCStatusComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(StatusList.Num() <= 0)
		return;

	for(UCStatusBase* base : StatusList)
	{
		base->Tick();
	}
}

void UCStatusComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}
