#pragma once

#include "CoreMinimal.h"
#include "../GameEnums/CStateEnum.h"
#include "IStateComponentVisitor.h"

class PORTFOLIO_API FStateDebugVisitor : public IStateComponentVisitor
{
public:
	FString Name;

	EStateType CurrentState = EStateType::Idle;
	EStateType PrevState = EStateType::Idle;
    
	ESubStateType CurrentSubState = ESubStateType::None;
	ESubStateType PrevSubState = ESubStateType::None;

	EHitResponseType CurrentHit = EHitResponseType::Normal;
	EHitResponseType PrevHit = EHitResponseType::Normal;

public:
	virtual void Visit(class UCStateComponent* StateComp) override;
};
