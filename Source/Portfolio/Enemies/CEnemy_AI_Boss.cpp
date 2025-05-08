#include "../Enemies/CEnemy_AI_Boss.h"
#include "../Global.h"
#include "Portfolio/Components/CAIBehaviorComponent.h"
#include "Portfolio/Components/CHitMotionComponent.h"
#include "Portfolio/Components/CMoveComponent.h"

#include "Portfolio/Components/CStatusComponent.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Stamina/CHealthStatus.h"
#include "Portfolio/Stamina/CStunStatus.h"
#include "Portfolio/Weapons/SubActions/CSubAction_Katana.h"


void ACEnemy_AI_Boss::BeginPlay()
{
	Super::BeginPlay();

	State->SetSuperArmorMode();
	
}

void ACEnemy_AI_Boss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

float ACEnemy_AI_Boss::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());

	if(State->IsDeadMode())
		return 0;

	IITeam* team = Cast<IITeam>(EventInstigator->GetPawn());
	if(team->GetTeamID() == TeamID)
		return 0;

	bool bCheck = true;
	bCheck &= State->IsSuperArmorMode();
	bCheck &= (State->IsDownMode() == false);
	bCheck &= IsInFront();
	bCheck &= (State->IsActionMode() == false);
	
	if(bCheck)
	{
		UCSubAction* subAction = Weapon->GetSubAction();
		UCSubAction_Katana* subActionKatana = Cast<UCSubAction_Katana>(subAction);
		if(!!subActionKatana)
		{
			if(State->IsGuardParryMode() && IsInFront() == true)
			{
				UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
				behavior->SetGuardMode();
				Stamina->GetStunStatus()->Decrease(5);
				subActionKatana->PlayGuard(DamageData.Attacker);
				return 0;
			}
			else
			{
				float randomValue = FMath::FRand();
				if(randomValue <= 0.7f)
				{
					UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
					behavior->SetGuardMode();
					Stamina->GetStunStatus()->Decrease(5);
					subActionKatana->PlayGuard(DamageData.Attacker);
					return 0;
				}
			}
		}
	}

	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACEnemy_AI_Boss::HitLaunch(FDoHitData* InHitData)
{
	if(Stamina->GetHealthStatus()->IsDead() || State->IsDeadMode())
		return;

	if(State->IsSuperArmorMode() == true)
		return;
	
	float XYLaunch = 0.0f;
	float ZLaunch = 0.0f;

	FVector start = GetActorLocation();
	FVector target = DamageData.Attacker->GetActorLocation();
	FVector direction = CalculateLaunchDirection(start, target, DamageData.Attacker->GetActorForwardVector() ,InHitData->AttackProperty.bIsExplosiveAttack);

	ApplyLaunchForce(direction, InHitData->LaunchData.Launch * BossKnockbackScale, 0.0f);
}

void ACEnemy_AI_Boss::End_DoAction()
{
	Super::End_DoAction();
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
	behavior->SetActionTime();	
}

bool ACEnemy_AI_Boss::CanBeAssassinated(EWeaponType InType, bool IsBack)
{
	if(State->IsDeadMode())
		return false;

	if(InType == EWeaponType::Katana)
	{
		if(IsBack)
			return false;

		return true;
	}
	
	return false;
}

void ACEnemy_AI_Boss::OnAssassinated(ACharacter* Assassin, FExecutionData& InData, EWeaponType InType)
{
	
	FVector AdjustedOffset = Assassin->GetActorForwardVector() * InData.ExecutionOffset.X +
								 Assassin->GetActorRightVector() * InData.ExecutionOffset.Y +
								 FVector(0.0f, 0.0f, InData.ExecutionOffset.Z);
	
	FVector NewLocation = Assassin->GetActorLocation() + AdjustedOffset;
	SetActorLocation(NewLocation);
	
	FRotator NewRotation = Assassin->GetActorRotation() + FRotator(0.0f, 180.0f, 0.0f);
	SetActorRotation(NewRotation);

	Stamina->GetStunStatus()->ResetStun();
	Stamina->GetHealthStatus()->Decrease(InData.Damage);
	if(Stamina->GetHealthStatus()->IsDead())
	{
		HitMotion->SetDeadMontage((EDeadType)InData.Type,InType); // 사망 몽타주를 변경
		State->SetDeadMode();
		
		return;
	}

	HitMotion->HitReactionAssassinated(InType,InData.Type);
	State->SetDamagedMode();
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
	behavior->SetDamagedMode();
}

FExecutionMontageData* ACEnemy_AI_Boss::GetLastAssassinMontage(float InDamage)
{
	if(Stamina->GetHealthStatus()->IsDeadAfterDamage(InDamage) == true)
	{
		return HitMotion->GetLastExecution();
	}

	return nullptr;
}


void ACEnemy_AI_Boss::ReceiveGuardCounter()
{
	// 보스 가드에 밀리지 않게
}
