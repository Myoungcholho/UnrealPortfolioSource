#include "../Components/CHitMotionComponent.h"
#include "../Global.h"

#include "GameFramework/Character.h"
#include "Portfolio/Character/CEnemy.h"
#include "Portfolio/Character/CHitMontageAsset.h"
#include "Portfolio/Character/CPlayer.h"
#include "Portfolio/Enemies/CDataAsset_MonsterDefinitions.h"

UCHitMotionComponent::UCHitMotionComponent()
{
	
}

void UCHitMotionComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	CheckNull(Owner);
	
	IICharacterLanded* landed = Cast<IICharacterLanded>(Owner);
	CheckNull(landed);
	
	landed->GetOnLandedDelegate().AddUObject(this, &UCHitMotionComponent::Landed);
}

void UCHitMotionComponent::HitReactionAssassinated(EWeaponType InWeaponType, EExecutionType InExecutionType)
{
	switch (InWeaponType)
	{
	case EWeaponType::Katana:
		if(InExecutionType == EExecutionType::BackStab)
		{
			Owner->PlayAnimMontage(DataAsset->BacktabMontage_Hit.PlayMontage,DataAsset->BacktabMontage_Hit.PlayRate);
		}
		else if(InExecutionType == EExecutionType::Groggy)
		{
			Owner->PlayAnimMontage(DataAsset->GroggyAssainationMontage_Hit.PlayMontage,DataAsset->GroggyAssainationMontage_Hit.PlayRate);
		}
		break;
	case EWeaponType::Bow:
		if(InExecutionType == EExecutionType::BackStab)
		{
			Owner->PlayAnimMontage(DataAsset->BowBackstabMontage_Hit.PlayMontage,DataAsset->BowBackstabMontage_Hit.PlayRate);
		}
		break;
	}
}

void UCHitMotionComponent::HitReaction(ACharacter* InAttacker,FDoHitData* InHitData,EWeaponType InType, EMonsterType Monster)
{
	CheckNull(Owner);
	UCStateComponent* State = FHelpers::GetComponent<UCStateComponent>(Owner);
	CheckNull(State);
	//State->PrintHitReactionMode();
	
	if(State->IsSuperArmorMode())
		return;

	if(State->IsInvincibleMode())
		return;
	
	switch (Monster)
	{
	case EMonsterType::Boss:
		BossMonsterHit(InAttacker,InHitData,State,InType);
		break;
	case EMonsterType::Normal:
		NormalMonsterHit(InHitData,State,InType);
		break;
	default:
		break;
	}
}

// 죽음 몽타주 처리, SetDeadMontage로 변경 가능
void UCHitMotionComponent::Dead()
{
	CheckNull(Owner);
	UCStateComponent* State = FHelpers::GetComponent<UCStateComponent>(Owner);
	CheckNull(State);
	
	if(State->IsAirborneMode())
		return;

	if(State->IsDownMode())
	{
		if(!!DataAsset->Lying_Dead_Montage.PlayMontage)
			Owner->PlayAnimMontage(DataAsset->Lying_Dead_Montage.PlayMontage,DataAsset->Lying_Dead_Montage.PlayRate);
		return;
	}

	switch(DeadWeaponType)
	{
	case EWeaponType::Katana:
		if(DeadType == EDeadType::Groggy)
		{
			if(!!DataAsset->GroggyAssainationMontage.PlayMontage)
				Owner->PlayAnimMontage(DataAsset->GroggyAssainationMontage.PlayMontage,DataAsset->GroggyAssainationMontage.PlayRate);
		}
		if(DeadType == EDeadType::BackStab)
		{
			if(!!DataAsset->BacktabMontage.PlayMontage)
				Owner->PlayAnimMontage(DataAsset->BacktabMontage.PlayMontage,DataAsset->BacktabMontage.PlayRate);
		}
		if(DeadType == EDeadType::LastExecution)
		{
			if(!!DataAsset->GroggyAssainationMontage.PlayMontage)
				Owner->PlayAnimMontage(DataAsset->GroggyAssainationMontage.PlayMontage,DataAsset->GroggyAssainationMontage.PlayRate);
		}
		break;
	case EWeaponType::Bow:
		if(DeadType == EDeadType::Groggy)
		{
			
		}
		if(DeadType == EDeadType::BackStab)
		{
			if(!!DataAsset->BowBackstabMontage.PlayMontage)
				Owner->PlayAnimMontage(DataAsset->BowBackstabMontage.PlayMontage,DataAsset->BowBackstabMontage.PlayRate);
		}
		break;
	case EWeaponType::Max:
		if(!!DataAsset->BasicDeadMontage.PlayMontage)
			Owner->PlayAnimMontage(DataAsset->BasicDeadMontage.PlayMontage,DataAsset->BasicDeadMontage.PlayRate);
		break;
	}
	
}

void UCHitMotionComponent::SetDeadMontage(EDeadType InType,EWeaponType InWeaponType)
{
	DeadType = InType;
	DeadWeaponType = InWeaponType;
}

void UCHitMotionComponent::ReceiveGuardCounter()
{
	CheckNull(Owner);
	UCStateComponent* State = FHelpers::GetComponent<UCStateComponent>(Owner);
	CheckNull(State);
	
	if(!!DataAsset->ReceiveGuardData.GuardHitMontage)
	{
		State->SetGuardImpactMode();
		Owner->PlayAnimMontage(DataAsset->ReceiveGuardData.GuardHitMontage,DataAsset->ReceiveGuardData.GuardHit_PlayRate);
	}
}

void UCHitMotionComponent::Landed(const FHitResult& Hit)
{
	CheckNull(Owner);
	UCStateComponent* State = FHelpers::GetComponent<UCStateComponent>(Owner);
	CheckNull(State);
	
	if(State->IsAirborneMode() == false)
		return;
	if(State->IsDeadMode() == true)
	{
		if(!!DataAsset->AirDeadMontage.PlayMontage)
			Owner->PlayAnimMontage(DataAsset->AirDeadMontage.PlayMontage,DataAsset->AirDeadMontage.PlayRate);
		
		return;
	}
	
	if(!!DataAsset->HitLandingMontage.PlayMontage)
		Owner->PlayAnimMontage(DataAsset->HitLandingMontage.PlayMontage,DataAsset->HitLandingMontage.PlayRate);
}

void UCHitMotionComponent::NormalMonsterHit(FDoHitData* InHitData,class UCStateComponent* InState, EWeaponType InType)
{
	if(InState->IsDownMode())
	{
		if(!!DataAsset->KnockDownMontage.PlayMontage)
			Owner->PlayAnimMontage(DataAsset->KnockDownMontage.PlayMontage,DataAsset->KnockDownMontage.PlayRate);
		return;
	}

	if(InState->IsAirborneMode())
	{
		if(!!DataAsset->AirDeadMontage.PlayMontage)
			Owner->PlayAnimMontage(DataAsset->AirHitMontage.PlayMontage,DataAsset->AirHitMontage.PlayRate);
		return;
	}
	
	int32 attackIndex = InHitData->HitAnimationData.AttackIndex;
	
	switch(InHitData->HitAnimationData.AttackHitType)
	{
	case EAttackHitType::ComboAttack:
		{
			if(InType == EWeaponType::Katana)
				Owner->PlayAnimMontage(DataAsset->HitMontages[attackIndex].PlayMontage,DataAsset->HitMontages[attackIndex].PlayRate);
			if(InType == EWeaponType::Bow)
				Owner->PlayAnimMontage(DataAsset->HitMontages_Bow[attackIndex].PlayMontage,DataAsset->HitMontages_Bow[attackIndex].PlayRate);
			if(InType == EWeaponType::Club)
				Owner->PlayAnimMontage(DataAsset->HitMontages[attackIndex].PlayMontage,DataAsset->HitMontages[attackIndex].PlayRate);
			break;
		}
	case EAttackHitType::CommandAttack:
		{
			Owner->PlayAnimMontage(DataAsset->HitCommandMontages[attackIndex].PlayMontage,DataAsset->HitCommandMontages[attackIndex].PlayRate);
			break;
		}
	case EAttackHitType::SpecialAttack:
		
		break;
	}
}

void UCHitMotionComponent::BossMonsterHit(ACharacter* InAttacker,FDoHitData* InHitData, class UCStateComponent* InState, EWeaponType InType)
{
	// 히트된 방향 구하고 적절한 몽타주 실행하기
	CheckNull(Owner);

	FVector direction = (InAttacker->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal();
	FVector forward = Owner->GetActorForwardVector();

	float dot = FVector::DotProduct(forward,direction);
	float cross = FVector::CrossProduct(forward,direction).Z;

	if (dot > 0.7f)
		Owner->PlayAnimMontage(DataAsset->ForwardMontage.PlayMontage,DataAsset->ForwardMontage.PlayRate);
	else if (dot < -0.7f)
		Owner->PlayAnimMontage(DataAsset->BackwardMontage.PlayMontage,DataAsset->BackwardMontage.PlayRate);
	else if (cross > 0)
		Owner->PlayAnimMontage(DataAsset->LeftMontage.PlayMontage,DataAsset->LeftMontage.PlayRate);
	else
		Owner->PlayAnimMontage(DataAsset->RightMontage.PlayMontage,DataAsset->RightMontage.PlayRate);
	
	/*FString str ="";
	str.Append("Dot :");
	str.Append(FString::SanitizeFloat(dot));
	FLog::Log(str);*/

	/*str ="";
	str.Append("Cross :");
	str.Append(FString::SanitizeFloat(cross));*/
	//FLog::Log(cross);
}

