#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"
#include "Portfolio/Weapons/CWeaponStructures.h"

class AIDEBUGGER_API AIStateDebugger : public FGameplayDebuggerCategory
{
public:
	AIStateDebugger();
	~AIStateDebugger();

public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

public:
	void CollectData(APlayerController* OwnerPC, AActor* DebufActor); __override;
	void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
	struct FDebugData
	{
		bool bDraw = false;
		
		FString Name;
		
		EWeaponType CurrentWeapon;
		EWeaponType PrevWeapon;

		EStateType CurrentState;
		EStateType PrevState;
		ESubStateType CurrentSubState;
		ESubStateType PrevSubState;
		EHitResponseType CurrentHitResponse;
		EHitResponseType PrevHitResponse;
		//uint8 StatusEffect;
	};

private:
	FDebugData PlayerData;
	FDebugData AIData;
	
};
