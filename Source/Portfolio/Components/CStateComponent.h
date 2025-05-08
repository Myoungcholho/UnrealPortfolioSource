#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../GameEnums/CStateEnum.h"
#include "CStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged,EStateType,InPrevType,EStateType,InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSubStateTypeChanged,ESubStateType,InPrevType,ESubStateType,InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitResponseTypeChanged,EHitResponseType,InPrevType,EHitResponseType,InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatusEffectChanged,uint8,InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode(){return Current == EStateType::Idle;}
	FORCEINLINE bool IsEvade(){return Current == EStateType::Evade;}
	FORCEINLINE bool IsActionMode(){return Current == EStateType::Action;}
	FORCEINLINE bool IsDeadMode(){return Current == EStateType::Dead;}
	FORCEINLINE bool IsDamagedMode(){return Current == EStateType::Damaged;}

	FORCEINLINE bool IsGuardingMode(){return Current == EStateType::Guarding;}
	FORCEINLINE bool IsGuardImpactMode(){return Current == EStateType::GuardImpact;}
	FORCEINLINE bool IsGuardBreakMode(){return Current == EStateType::GuardBreak;}
	FORCEINLINE bool IsGuardParryMode(){return Current == EStateType::GuardParry;}

	FORCEINLINE bool IsAssassinationMode(){return Current == EStateType::Assassination;}
	FORCEINLINE bool IsGroggyMode(){return Current == EStateType::Groggy;}

	// PlugIn UseMethod
	FORCEINLINE EStateType GetCurrentType() const {return Current;}
	FORCEINLINE EStateType GetPrevType() const {return PrevType;}
	FORCEINLINE ESubStateType GetCurrentSubType() const{return CurrentSubState;}
	FORCEINLINE ESubStateType GetPrevSubType() const{return PrevSubState;}
	FORCEINLINE EHitResponseType GetCurrentHitResponseType() const{return CurrentThird;}
	FORCEINLINE EHitResponseType GetPrevHitResponseType() const{return PrevThird;}
	FORCEINLINE uint8 GetActiveStatusEffects() const {return ActiveStatusEffects;}
//SubState
public:
	FORCEINLINE bool IsNoneMode(){return CurrentSubState == ESubStateType::None;}
	FORCEINLINE bool IsJustGuardMode(){return CurrentSubState == ESubStateType::JustGuard;}
	FORCEINLINE bool IsDownMode(){return CurrentSubState == ESubStateType::Down;}
	FORCEINLINE bool IsAirborneMode(){return CurrentSubState == ESubStateType::Airborne;}
	FORCEINLINE bool IsUnblockableMode(){return CurrentSubState == ESubStateType::UnBlock;}

//ThirdState
	FORCEINLINE bool IsNormalMode(){return CurrentThird == EHitResponseType::Normal;}
	FORCEINLINE bool IsSuperArmorMode(){return CurrentThird == EHitResponseType::SuperArmor;}
	FORCEINLINE bool IsAntiAirborneMode(){return CurrentThird == EHitResponseType::AntiAirborne;}
	FORCEINLINE bool IsInvincibleMode(){return CurrentThird == EHitResponseType::Invincible;}

//EStatusEffect
	// FORCEINLINE 할수 없음. 연산이 필요함
	
//SubAction
public:
	FORCEINLINE bool IsSubActionMode(){ return bSubAction == true; }
	
public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetIdleMode();
	void SetEvadeMode();
	void SetEquipMode();
	void SetDamagedMode();
	void SetActionMode();
	void SetDeadMode();
	void SetGuardingMode();
	void SetGuardImpactMode();
	void SetGuardBreakMode();
	void SetGuardParryMode();
	void SetAssassinationMode();
	void SetGroggyMode();
	
//SubState
public:
	void SetSubNoneMode();
	void SetSubJustGuardMode();
	void SetSubDownMode();
	void SetSubAirborneMode();
	void SetSubUnBlockableMode();

//ThirdState
public:
	void SetNormalMode();
	void SetSetAntiAirborneMode();
	void SetSuperArmorMode();
	void SetInvincibleMode();
	void PrintHitReactionMode();
// EStatusEffect
public:
	void AddStatusEffect(EStatusEffect Effect);
	void RemoveStatusEffect(EStatusEffect Effect);
	bool HasStatusEffect(EStatusEffect Effect);
	void ClearStatusEffect();

// 우클릭 subAction
public:
	void OnSubAction();
	void OffSubAction();
	
private:
	void ChangeType(EStateType InType);
	void SubChangeType(ESubStateType InType);
	void HitResponseChangeType(EHitResponseType InType);
	
public:
	FStateTypeChanged OnStateTypeChanged;
	FSubStateTypeChanged OnSubStateTypeChanged;
	FHitResponseTypeChanged OnHitResponseTypeChanged;
	FStatusEffectChanged OnStatusEffectChanged;

public:
	void Accept(class IStateComponentVisitor* Visitor); 
	
private:
	EStateType Current;
	EStateType PrevType;
// SubState
private:
	ESubStateType CurrentSubState;
	ESubStateType PrevSubState;
// ThirdState
	EHitResponseType CurrentThird;
	EHitResponseType PrevThird;
// StatusEffect
	uint8 ActiveStatusEffects;
// subaction state
	bool bSubAction = false;
};
