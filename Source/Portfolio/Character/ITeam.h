// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITeam.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UITeam : public UInterface
{
	GENERATED_BODY()
};

class PORTFOLIO_API IITeam
{
	GENERATED_BODY()
public:
	virtual void SetTeamID(uint8 InTeamID)=0;
	virtual uint8 GetTeamID()=0;
};
