#include "../Components/CStateComponent.h"

#include "IStateComponentVisitor.h"
#include "../Global.h"

UCStateComponent::UCStateComponent()
{
	
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UCStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetEvadeMode()
{
	ChangeType(EStateType::Evade);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetDamagedMode()
{
	
	ChangeType(EStateType::Damaged);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetGuardingMode()
{
	ChangeType(EStateType::Guarding);
}

void UCStateComponent::SetGuardImpactMode()
{
	ChangeType(EStateType::GuardImpact);
}

void UCStateComponent::SetGuardBreakMode()
{
	ChangeType(EStateType::GuardBreak);
}

void UCStateComponent::SetGuardParryMode()
{
	ChangeType(EStateType::GuardParry);
}

void UCStateComponent::SetAssassinationMode()
{
	ChangeType(EStateType::Assassination);
}

void UCStateComponent::SetGroggyMode()
{
	ChangeType(EStateType::Groggy);
}

/////////////////////////////////////////////////
void UCStateComponent::SetSubNoneMode()
{
	SubChangeType(ESubStateType::None);
}

void UCStateComponent::SetSubJustGuardMode()
{
	SubChangeType(ESubStateType::JustGuard);
}

void UCStateComponent::SetSubDownMode()
{
	SubChangeType(ESubStateType::Down);
}

void UCStateComponent::SetSubAirborneMode()
{
	SubChangeType(ESubStateType::Airborne);
}

void UCStateComponent::SetSubUnBlockableMode()
{
	SubChangeType(ESubStateType::UnBlock);
}

void UCStateComponent::SetNormalMode()
{
	HitResponseChangeType(EHitResponseType::Normal);
}

void UCStateComponent::SetSetAntiAirborneMode()
{
	HitResponseChangeType(EHitResponseType::AntiAirborne);
}

void UCStateComponent::SetSuperArmorMode()
{
	HitResponseChangeType(EHitResponseType::SuperArmor);
}

void UCStateComponent::SetInvincibleMode()
{
	HitResponseChangeType(EHitResponseType::Invincible);
}

void UCStateComponent::PrintHitReactionMode()
{
	if(IsNormalMode() == true)
		FLog::Log("IsNormalMode");
	if(IsSuperArmorMode() == true)
		FLog::Log("IsSuperArmorMode");
	if(IsAntiAirborneMode() == true)
		FLog::Log("IsAntiAirborneMode");
	if(IsInvincibleMode() == true)
		FLog::Log("IsInvincibleMode");
}

void UCStateComponent::AddStatusEffect(EStatusEffect Effect)
{
	ActiveStatusEffects |= static_cast<uint8>(Effect);

	if(OnStatusEffectChanged.IsBound())
		OnStatusEffectChanged.Broadcast(ActiveStatusEffects);
}

void UCStateComponent::RemoveStatusEffect(EStatusEffect Effect)
{
	ActiveStatusEffects &= ~static_cast<uint8>(Effect);
	
	if(OnStatusEffectChanged.IsBound())
    		OnStatusEffectChanged.Broadcast(ActiveStatusEffects);
}

bool UCStateComponent::HasStatusEffect(EStatusEffect Effect)
{
	return (ActiveStatusEffects & static_cast<uint8>(Effect)) != 0;
}

void UCStateComponent::ClearStatusEffect()
{
	ActiveStatusEffects = 0;

	if(OnStatusEffectChanged.IsBound())
		OnStatusEffectChanged.Broadcast(ActiveStatusEffects);
}

void UCStateComponent::OnSubAction()
{
	bSubAction = true;
}

void UCStateComponent::OffSubAction()
{
	bSubAction = false;
}

void UCStateComponent::ChangeType(EStateType InType)
{
	if(IsDeadMode())
		return;

	//if(InType == EStateType::Idle && Current == EStateType::Damaged)
	//	FLog::Log("what?");
	
	PrevType = Current;
	Current = InType;
	
	if(OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(PrevType,InType);
}

void UCStateComponent::SubChangeType(ESubStateType InType)
{
	/*FString str = "Type :";
	str.Append(FString::FromInt(static_cast<int32>(InType)));
	FLog::Log(str);

	if(InType == ESubStateType::None && CurrentSubState == ESubStateType::UnBlock)
		FLog::Log(str);*/
	
	PrevSubState = CurrentSubState;
	CurrentSubState = InType;
	
	if(OnSubStateTypeChanged.IsBound())
		OnSubStateTypeChanged.Broadcast(PrevSubState,InType);
}

void UCStateComponent::HitResponseChangeType(EHitResponseType InType)
{
	PrevThird = CurrentThird;
	CurrentThird = InType;

	if(OnHitResponseTypeChanged.IsBound())
		OnHitResponseTypeChanged.Broadcast(PrevThird,InType);
}

void UCStateComponent::Accept(IStateComponentVisitor* Visitor)
{
	if (Visitor)
		Visitor->Visit(this);
}
