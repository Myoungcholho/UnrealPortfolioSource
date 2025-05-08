#include "../../Weapons/DoActions/CDoAction.h"
#include "../../Global.h"

#include "GameFramework/Character.h"
#include "../../Components/CStateComponent.h"
#include "../../Components/CMoveComponent.h"


UCDoAction::UCDoAction()
{
	
}

void UCDoAction::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCEquipment* InEquipment,
	const TArray<FDoActionData>& InDoActionDatas, const TArray<FDoHitData>& InHitDatas,
	const TArray<FDoActionData>& InCommandActionDatas, const TArray<FDoHitData>& InCommandHitDatas,
	const TArray<FDoActionData>& InDoAirActionDatas, const TArray<FDoHitData>& InHitAirDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	Movement = FHelpers::GetComponent<UCMoveComponent>(OwnerCharacter);
	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;

	DoCommandActionDatas = InCommandActionDatas;
	HitCommandDatas = InCommandHitDatas;

	DoAirActionDatas = InDoAirActionDatas;
	HitAirDatas = InHitAirDatas;
}

void UCDoAction::DoAction()
{
	bInAction = true;
	State->SetActionMode();
}

void UCDoAction::DoAction(int32 InPlayIndex)
{
	bInAction = true;
	State->SetActionMode();
}

void UCDoAction::DoCommandAction(int32 InPlayIndex)
{
	if(InPlayIndex >= DoCommandActionDatas.Num())
		return;

	bCommandAction = true;
	State->SetActionMode();
	CommandIndex = InPlayIndex;
	DoCommandActionDatas[InPlayIndex].DoAction(OwnerCharacter);
}


const TArray<FDoActionData>& UCDoAction::GetCommandActionDatas() const
{
	return DoCommandActionDatas;
}

void UCDoAction::Begin_DoAction()
{
	if(State->IsDamagedMode())
		return;
	if(State->IsGuardParryMode())
		return;
	
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bInAction = false;
	bBeginAction = false;
	bCommandAction = false;
	
	bBeginAirAction = false;
	bAirAction = false;
	//FLog::Log("bAirAction false");
	if (!(State->IsDamagedMode() || State->IsGuardParryMode()))
		State->SetIdleMode();

	Movement->Move();
	Movement->DisableFixedCamera();
}

void UCDoAction::Begin_Air_DoAction()
{
	if(State->IsDamagedMode())
		return;
	if(State->IsGuardParryMode())
		return;
	
	bBeginAirAction = true;
	bAirAction = true;
	//FLog::Log("bAirAction True");
}