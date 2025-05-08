#include "../../Weapons/SubActions/CSubAction_Katana.h"

#include "AIController.h"
#include "CDataAsset_Katana_Guard.h"
#include "../../Global.h"
#include "GameFramework/Character.h"
#include "Portfolio/Components/CMoveComponent.h"
#include "Portfolio/Components/CStateComponent.h"

void UCSubAction_Katana::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment,
                                   class UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
	FHelpers::GetAssetDynamic(&DataAsset,"/Script/Portfolio.CDataAsset_Katana_Guard'/Game/Weapons/Katana/DA_Katana_Guard_Fallen.DA_Katana_Guard_Fallen'");
	
}

void UCSubAction_Katana::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	
}

void UCSubAction_Katana::Begin_SubAction()
{
	Super::Begin_SubAction();

	State->SetSubJustGuardMode();
	State->SetGuardingMode();
	Movement->OnRun();
}

void UCSubAction_Katana::End_SubAction()
{
	Super::End_SubAction();
	State->SetSubNoneMode();
}

// 무기 내리기 시작할때
void UCSubAction_Katana::Begin_UnSubAction()
{
	Super::Begin_UnSubAction();
	
}

// 무기 내리기 끝날때
void UCSubAction_Katana::End_UnSubAction()
{
	Super::End_UnSubAction();
	bInAction = false;
	if(State->IsDamagedMode() == false)
		State->SetIdleMode();
	Movement->Move();
	Reset();
}

// 마우스 우클릭 시작 시 호출
void UCSubAction_Katana::Pressed()
{
	Super::Pressed();

	DataAsset->GuardActiveData.PlayBeginGuard(Owner);
}

// 마우스 우클릭 끝날 시 호출
void UCSubAction_Katana::Released()
{
	Super::Released();
	
	// 가드 모드에서 정상적인 해제
	if(State->IsGuardingMode())
	{
		DataAsset->GuardActiveData.PlayEndGuard(Owner);
		return;
	}
	
	Begin_UnSubAction();
	End_UnSubAction();
}

void UCSubAction_Katana::OnUnEquip()
{
	Super::OnUnEquip();
}

void UCSubAction_Katana::PlayGuard(class ACharacter* InAttacker)
{
	if(DataAsset->GuardDatas.Num() <= GuardIndex)
		return;

	State->SetGuardParryMode();
	
	// non반경 상태로 변경
	End_SubAction();

	// 움직임 정지
	Movement->Stop();
	
	if(GuardStack > 3)
	{
		PlayGuardBroken();
		return;
	}
	
	DataAsset->GuardDatas[GuardIndex].PlayGuardMotion(Owner);
	// 플레이어인 경우에만..
	if(InAttacker->Tags.Contains("Player"))
		DataAsset->GuardDatas[GuardIndex].PlayHitStop(Owner);

	//Launch
	{
		FVector start = Owner->GetActorLocation();
		FVector target = InAttacker->GetActorLocation();
		target.Z = start.Z;

		FVector direction = (target - start);
		direction.Normalize();
		
		FRotator lookRot = UKismetMathLibrary::FindLookAtRotation(start, target);
		Owner->SetActorRotation(lookRot);
		
		float launch = DataAsset->GuardDatas[GuardIndex].Launch;
		if(launch > 0.0f)
			Owner->LaunchCharacter(-direction* launch,false,false);
	}

	++GuardIndex;
	++GuardStack;
	
	// 가드 순회
	if(GuardIndex >= DataAsset->GuardDatas.Num())
		GuardIndex = 0;
}

void UCSubAction_Katana::PlayGuardBroken()
{
	if(DataAsset->GuardBrokenData.GuardBrokenMontage == nullptr)
		return;

	State->SetGuardBreakMode();
	State->SetSubUnBlockableMode();
	DataAsset->GuardBrokenData.PlayBrokenMotion(Owner);
	Movement->Move();
}

void UCSubAction_Katana::End_Parry()
{
	Reset();
	Movement->Move();
	if(bInAction == true)
		State->SetGuardingMode();
	else
		State->SetIdleMode();
}

void UCSubAction_Katana::End_Broken()
{
	State->SetIdleMode();
	State->SetSubNoneMode();
	Reset();
}


void UCSubAction_Katana::Reset()
{
	GuardIndex = 0;
	GuardStack = 0;
}




