#include "../Debugger/AIStateDebugger.h"

#include "CanvasItem.h"
#include "Portfolio/Character/CPlayer.h"
#include "Portfolio/Global.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Components/FStateDebugVisitor.h"

AIStateDebugger::AIStateDebugger()
{
	bShowOnlyWithDebugActor = false;
}

AIStateDebugger::~AIStateDebugger()
{
	
}

TSharedRef<FGameplayDebuggerCategory> AIStateDebugger::MakeInstance()
{
	return MakeShareable(new AIStateDebugger());
}

void AIStateDebugger::CollectData(APlayerController* OwnerPC, AActor* DebufActor)
{
	FGameplayDebuggerCategory::CollectData(OwnerPC, DebufActor);

	// === Player ===
	PlayerData.bDraw = false;
	
	ACharacter* player = OwnerPC->GetPawn<ACharacter>();
	CheckNull(player);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(player);
	CheckNull(state);
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(player);
	CheckNull(weapon);

	// Vistor
	FStateDebugVisitor PlayerVisitor;
	state->Accept(&PlayerVisitor);
	
	PlayerData.bDraw = true;
	PlayerData.Name = PlayerVisitor.Name;

	PlayerData.CurrentWeapon = weapon->GetCurrentType();
	PlayerData.PrevWeapon = weapon->GetPrevType();
	
	PlayerData.CurrentState = PlayerVisitor.CurrentState;
	PlayerData.PrevState = PlayerVisitor.PrevState;
	PlayerData.CurrentSubState = PlayerVisitor.CurrentSubState;
	PlayerData.PrevSubState = PlayerVisitor.PrevSubState;
	PlayerData.CurrentHitResponse = PlayerVisitor.CurrentHit;
	PlayerData.PrevHitResponse = PlayerVisitor.PrevHit;
	//PlayerData.StatusEffect = state->GetActiveStatusEffects();

	// === AI ===
	AIData.bDraw = false;
	
	ACharacter* ai = Cast<ACharacter>(DebufActor);
	CheckNull(ai);
	UCStateComponent* state_ai = FHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(state_ai);
	UCWeaponComponent* weapon_ai = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNull(weapon_ai);

	AIData.bDraw = true;
	AIData.Name = ai->GetName();

	AIData.CurrentWeapon = weapon_ai->GetCurrentType();
	AIData.PrevWeapon = weapon_ai->GetPrevType();
	
	AIData.CurrentState = state_ai->GetCurrentType();
	AIData.PrevState = state_ai->GetPrevType();
	AIData.CurrentSubState = state_ai->GetCurrentSubType();
	AIData.PrevSubState = state_ai->GetPrevSubType();
	AIData.CurrentHitResponse = state_ai->GetCurrentHitResponseType();
	AIData.PrevHitResponse = state_ai->GetPrevHitResponseType();
}

void AIStateDebugger::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);
	
	FCanvasTileItem item(FVector2D(10,20),FVector2D(450,100),FLinearColor(0,0,0,0.25f));
	item.BlendMode = SE_BLEND_AlphaBlend;
	CanvasContext.DrawItem(item,CanvasContext.CursorX, CanvasContext.CursorY);
	
	// === Canvas ===
	
	if(PlayerData.bDraw == true)
	{
		CanvasContext.Printf(FColor::Green, L" -- Player -- ");
		CanvasContext.Printf(FColor::White, L"Name : %s", *PlayerData.Name);
		CanvasContext.Printf(FColor::White, L"Weapon : %s", *UEnum::GetValueAsString(PlayerData.CurrentWeapon));
		CanvasContext.Printf(FColor::White, L"State : %s → %s",*UEnum::GetValueAsString(PlayerData.PrevState),*UEnum::GetValueAsString(PlayerData.CurrentState));
		CanvasContext.Printf(FColor::White, L"SubState : %s → %s",*UEnum::GetValueAsString(PlayerData.PrevSubState),*UEnum::GetValueAsString(PlayerData.CurrentSubState));
		CanvasContext.Printf(FColor::White, L"HitResponse : %s → %s",*UEnum::GetValueAsString(PlayerData.PrevHitResponse),*UEnum::GetValueAsString(PlayerData.CurrentHitResponse));
		//CanvasContext.Printf(FColor::White, L"StatusEffect : %s", *UEnum::GetValueAsString(PlayerData.StatusEffect));
	}

	// === Canvas ===
	CanvasContext.CursorY += 20;
	
	FCanvasTileItem item2(FVector2D(10,20),FVector2D(450,100),FLinearColor(0,0,0,0.25f));
	item2.BlendMode = SE_BLEND_AlphaBlend;
	CanvasContext.DrawItem(item2,CanvasContext.CursorX, CanvasContext.CursorY);
	
	if(AIData.bDraw == true)
	{
		CanvasContext.Printf(FColor::Green, L" -- AI State -- ");
		CanvasContext.Printf(FColor::White, L"Name : %s", *AIData.Name);
		CanvasContext.Printf(FColor::White, L"Weapon : %s", *UEnum::GetValueAsString(AIData.CurrentWeapon));
		CanvasContext.Printf(FColor::White, L"State : %s → %s",*UEnum::GetValueAsString(AIData.PrevState),*UEnum::GetValueAsString(AIData.CurrentState));
		CanvasContext.Printf(FColor::White, L"SubState : %s → %s",*UEnum::GetValueAsString(AIData.PrevSubState),*UEnum::GetValueAsString(AIData.CurrentSubState));
		CanvasContext.Printf(FColor::White, L"HitResponse : %s → %s",*UEnum::GetValueAsString(AIData.PrevHitResponse),*UEnum::GetValueAsString(AIData.CurrentHitResponse));
	}
}
