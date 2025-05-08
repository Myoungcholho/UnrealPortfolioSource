#include "../Components/CWeaponComponent.h"

#include "GameFramework/Character.h"

//////////////
#include "CMoveComponent.h"
#include "CStateComponent.h"
#include "../Weapons/CWeaponAsset.h"
#include "Blueprint/UserWidget.h"
#include "Portfolio/Utilities/FHelpers.h"
#include "Portfolio/Utilities/FLog.h"
#include "Portfolio/Weapons/CEquipment.h"
#include "Portfolio/Weapons/CWeaponData.h"
#include "Portfolio/Weapons/Assassinations/CAssassination.h"
#include "Portfolio/Weapons/DoActions/CDoAction.h"
#include "Portfolio/Weapons/SubActions/CSubAction.h"
#include "Portfolio/Widget/CStateWidget.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = FHelpers::GetComponent<UCMoveComponent>(OwnerCharacter);
	
	for(int32 i=0; i<DataAssets.Num(); ++i)
	{
		if(DataAssets[i] == nullptr) continue;

		//DataAssets[i]->BeginPlay(OwnerCharacter);
		UCWeaponData* data = nullptr;
		DataAssets[i]->BeginPlay(OwnerCharacter,&data);

		Datas.Add(data);
	}
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(int i=0; i<Datas.Num();++i)
	{
		if(Datas[i] == nullptr) continue;

		UCDoAction* doAction = Datas[i]->GetDoAction();
		if(!!doAction)
		{
			doAction->Tick(DeltaTime);
		}

		UCSubAction* subAction = Datas[i]->GetSubAction();
		if(!!subAction)
		{
			subAction->Tick(DeltaTime);
		}
	}
	
}

UCWeaponData* UCWeaponComponent::GetWeaponData(EWeaponType InType)
{
	for(UCWeaponData* asset : Datas)
	{
		if(InType == asset->GetWeaponType())
			return asset;
	}
	
	return nullptr;
}

class ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(),nullptr);

	UCWeaponData* asset = GetWeaponData(CurrentType);
	CheckNullResult(asset,nullptr);
	
	return asset->GetAttachment();
}


class UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(),nullptr);

	UCWeaponData* asset = GetWeaponData(CurrentType);
	CheckNullResult(asset,nullptr);

	return asset->GetEquipment();
}

class UCEquipment* UCWeaponComponent::GetPrevEquipment()
{
	UCWeaponData* asset = GetWeaponData(PrevType);
	CheckNullResult(asset,nullptr);

	return asset->GetEquipment();
}

class UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);

	UCWeaponData* asset = GetWeaponData(CurrentType);
	CheckNullResult(asset,nullptr);

	return asset->GetDoAction();
}

// SubAction Data 얻어옴
class UCSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);

	UCWeaponData* asset = GetWeaponData(CurrentType);
	CheckNullResult(asset,nullptr);

	return asset->GetSubAction();
}

class UCSubAction* UCWeaponComponent::GetPrevSubAction()
{
	UCWeaponData* asset = GetWeaponData(PrevType);
	CheckNullResult(asset,nullptr);

	return asset->GetSubAction();
}

// Assassination 얻어옴
class UCAssassination* UCWeaponComponent::GetAssassination()
{
	CheckTrueResult(IsUnarmedMode(),nullptr);

	UCWeaponData* asset = GetWeaponData(CurrentType);
	CheckNullResult(asset,nullptr);

	return asset->GetAssassination();
}


///////////////////////////////////////////////////////////////////
void UCWeaponComponent::DoAction()
{
	CheckNull(State);
	if((State->IsIdleMode() == true || State->IsActionMode() == true) == false)
		return;
	
	UCDoAction* doAction = GetDoAction();

	if(!!doAction)
	{
		doAction->DoAction();
	}
}

void UCWeaponComponent::DoAction(int32 InPlayIndex)
{
	CheckNull(State);
	if((State->IsIdleMode() == true || State->IsActionMode() == true) == false)
		return;

	UCDoAction* doAction = GetDoAction();

	if(!!doAction)
	{
		doAction->DoAction(InPlayIndex);
	}
}

void UCWeaponComponent::CommandDoAction(int32 InIndex)
{
	CheckNull(State);
	if((State->IsIdleMode() == true || State->IsActionMode() == true) == false)
		return;

	UCDoAction* doAction = GetDoAction();

	if(!!doAction)
	{
		doAction->DoCommandAction(InIndex);
	}
}

// SetMode
void UCWeaponComponent::SetUnarmedMode()
{
	GetEquipment()->UnEquip(false);

	if(!!Movement)
		Movement->DisableControlRotation();
	
	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetKatanaMode()
{
	CheckFalse(IsIdleMode());
	
	if(!!Movement)
		Movement->EnableControlRotation();
	
	SetMode(EWeaponType::Katana);
}

void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());

	if(!!Movement)
		Movement->EnableControlRotation();

	SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::SetClubMode()
{
	CheckFalse(IsIdleMode());

	if(!!Movement)
		Movement->EnableControlRotation();

	SetMode(EWeaponType::Club);
}

bool UCWeaponComponent::IsIdleMode()
{
	return FHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	// 장착 중인 무기를 누른 경우
	if(CurrentType == InType)
	{
		SetUnarmedMode();
		return;
	}
	// 장착 중인 무기를 해제
	else if(IsUnarmedMode() == false)
	{
		GetEquipment()->UnEquip(true);
		//return; // 그냥 빠른 스위칭하려면 return 제거
	}

	UCWeaponData* asset = GetWeaponData(InType);
	if(!!asset)
	{
		asset->GetEquipment()->Equip();
		
		ChangeType(InType);
	}
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	PrevType = CurrentType;
	CurrentType = InType;

	if(OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(PrevType, InType);
}

void UCWeaponComponent::SubAction_Pressed()
{
	CheckNull(State);
	if((State->IsIdleMode() == true || State->IsActionMode() == true) == false)
	{
		FLog::Log("dont Pressed");
		return;
	}

	UCDoAction* doAction = GetDoAction();
	if(!!doAction)
		doAction->End_DoAction();
	
	UCSubAction* subAction = GetSubAction();

	if(!!subAction)
		subAction->Pressed();
}

void UCWeaponComponent::SubAction_Released()
{
	UCSubAction* subAction = GetSubAction();

	if(!!subAction)
	{
		subAction->Released();
	}
}

void UCWeaponComponent::DoAssassation()
{
	CheckNull(State);
	if((State->IsIdleMode() || State->IsActionMode()) == false)
		return;
	
	UCAssassination* assassination = GetAssassination();

	if(!!assassination)
	{
		assassination->DoAssassination();
	}
}

