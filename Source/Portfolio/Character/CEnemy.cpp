#include "../Character/CEnemy.h"

#include "ITeam.h"
#include "../Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Portfolio/Components/CMoveComponent.h"
#include "Portfolio/Components/CStateComponent.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "Portfolio/Weapons/SubActions/CSubAction.h"
#include "Components/BoxComponent.h"
#include "Portfolio/Components/CHitMotionComponent.h"
#include "Portfolio/Components/CItemDropComponent.h"
#include "Portfolio/Components/CStatusComponent.h"
#include "Portfolio/Enemies/CDataAsset_MonsterDefinitions.h"
#include "Portfolio/Managers/CGameModeBase.h"
#include "Portfolio/Managers/CHitEffectPostProcessController.h"
#include "Portfolio/Stamina/CHealthStatus.h"
#include "Portfolio/Stamina/CStunStatus.h"
#include "Portfolio/Weapons/CAttachment.h"
#include "Portfolio/Weapons/IWeaponType.h"

ACEnemy::ACEnemy()
{
	FHelpers::CreateActorComponent<UCMoveComponent>(this, &Movement, "Movement");
	FHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	FHelpers::CreateActorComponent<UCStatusComponent>(this,&Stamina,"Stamina");
	FHelpers::CreateActorComponent<UCLockonComponent>(this,&LockOn,"LockOn");
	FHelpers::CreateActorComponent<UCHitMotionComponent>(this,&HitMotion,"HitMotion");
	FHelpers::CreateActorComponent<UCItemDropComponent>(this,&ItemDrop,"ItemDrop");
	
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	FHelpers::GetAsset<USkeletalMesh>(&mesh,"/Script/Engine.SkeletalMesh'/Game/GhostSamurai_Bundle/GhostSamurai/Character/Mesh/SK_GhostSamurai.SK_GhostSamurai'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	FHelpers::GetClass<UAnimInstance>(&animInstance,"/Script/Engine.AnimBlueprint'/Game/Animations/ABP_Samurai.ABP_Samurai_C'");
	GetMesh()->SetAnimClass(animInstance);

	FHelpers::CreateComponent<UBoxComponent>(this, &BoxTrigger, "HitCorrection",GetCapsuleComponent());
	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxTrigger->SetBoxExtent(FVector(60.0f, 60.0f, 200.0f));
	BoxTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	
	//GetCharacterMovement()->RotationRate = FRotator(0,720,0);
	Tags.Add("Enemy");
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnWalk();
	Movement->DisableControlRotation(); // 기본은 가는 방향으로 회전X
	
	State->OnStateTypeChanged.AddDynamic(this,&ACEnemy::OnStateTypeChanged);
	State->OnSubStateTypeChanged.AddDynamic(this,&ACEnemy::OnSubStateTypeChanged);
}

float ACEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(State->IsDeadMode())
		return 0;
	
	IITeam* team = Cast<IITeam>(EventInstigator->GetPawn());
	if(team->GetTeamID() == TeamID)
		return 0;
	
	DamageData.Power = DamageAmount;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	if(State->IsSuperArmorMode() == false)
		State->SetDamagedMode();
	
	Damaged();
	
	return DamageAmount;
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch(InNewType)
	{
		case EStateType::Idle:
			Idle();
			break;
		case EStateType::Dead:
			Dead();
			break;
	}
}

void ACEnemy::OnSubStateTypeChanged(ESubStateType InPrevType, ESubStateType InNewType)
{
	switch(InNewType)
	{
	case ESubStateType::Down:
	case ESubStateType::Airborne:
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
		BoxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		// 눕거나 공중에 있을때 회전 막기
		Movement->DisableControlRotation();
		
		break;
	case ESubStateType::None:
	default:
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
		BoxTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 원래상태라면 회전가능하도록 변경
		Movement->EnableControlRotation();
		break;
	}
}

void ACEnemy::Damaged()
{
	if(DamageData.Attacker == GetOwner())
	{
		DamageData.Attacker = nullptr;
		DamageData.Causer = nullptr;
		DamageData.Event = nullptr;
		return;
	}

	IIWeaponType* weaponType = Cast<IIWeaponType>(DamageData.Causer);
	EWeaponType type = weaponType->RetruenWeaponType();
	EMonsterType monsterType = EMonsterType::Normal;
	
	// HP 차감 & Stun
	{
		Stamina->GetHealthStatus()->Decrease(DamageData.Power);
		Stamina->GetStunStatus()->Decrease(DamageData.Power * 2);
		
		DamageData.Power = 0;
	}
	
	// 데미지 피격 함수
	if(!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FDoHitData* hitData = DamageData.Event->HitData;
		
		if(!!MonsterData)
			monsterType = MonsterData->GetMonsterType();
		
		HitMotion->HitReaction(DamageData.Attacker,DamageData.Event->HitData,type,monsterType);
		 
		if(DamageData.Attacker->Tags.Contains("Player"))
		{
			hitData->PlayHitStop(this);
			// 방사형 블러
			if(type == EWeaponType::Katana)
			{
				ACGameModeBase* GameMode = Cast<ACGameModeBase>(GetWorld()->GetAuthGameMode());
				if (GameMode)
				{
					UCHitEffectPostProcessController* PostProcessController = GameMode->GetPostProcessController();
					if (PostProcessController)
					{
						PostProcessController->EnableHitEffect(hitData->HitEffectData.StopTime);
					}
				}
			}
		}

		FVector hitLocation = DamageData.Causer->GetActorLocation();
		hitData->PlayEffect(this,DamageData.Attacker,type,&hitLocation);
		hitData->PlayCameraShake(this);

		HitLaunch(hitData);
	}
	
	if(Stamina->GetHealthStatus()->IsDead())
	{
		State->SetDeadMode();

		return;
	}
	
	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;
	DamageData.Event = nullptr;
}

void ACEnemy::Landed(const FHitResult& Hit)
{
	if(OnLandedDelegate.IsBound())
		OnLandedDelegate.Broadcast(Hit);
}

FVector ACEnemy::CalculateLaunchDirection(const FVector& Start, const FVector& Target, const FVector& AttackerForward,bool bIsExplosive)
{
	FVector direction;

	if (!bIsExplosive)
		direction = -AttackerForward;
	else
		direction = (Target - Start);

	direction.Z = 0; // 수평 방향만 사용
	direction.Normalize();
	return direction;
}

void ACEnemy::ApplyLaunchForce(const FVector& Direction, float XYLaunch, float ZLaunch)
{
	if (FMath::IsNearlyZero(XYLaunch) && FMath::IsNearlyZero(ZLaunch))
		return;

	FVector launchForce = -Direction * XYLaunch;
	launchForce.Z += ZLaunch;

	// 기존 속도를 무시하고 새 속도 적용
	LaunchCharacter(launchForce, true, true);
	//FLog::Log(launchForce);
}

void ACEnemy::SetTargetRotation(const FVector& Start, const FVector& Target)
{
	FVector flatTarget = FVector(Target.X, Target.Y, Start.Z);
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Start, flatTarget));
}

void ACEnemy::Dead()
{
	if(OnBegin_Dead.IsBound())
		OnBegin_Dead.Broadcast();
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("FloorOnlyPreset"));

	ItemDrop->ItemSpawn();
	HitMotion->Dead();
}

void ACEnemy::End_Dead()
{
	if(OnEnd_Dead.IsBound())
		OnEnd_Dead.Broadcast();
	
	Destroy();
}

void ACEnemy::End_Damaged()
{
	State->SetIdleMode();
	State->SetSubNoneMode();
}

void ACEnemy::End_GuardImpact()
{
	State->SetIdleMode();
}


void ACEnemy::ReceiveGuardCounter()
{
	HitMotion->ReceiveGuardCounter();
}

void ACEnemy::SetTeamID(uint8 InTeamID)
{
	TeamID = InTeamID;
}

uint8 ACEnemy::GetTeamID()
{
	return TeamID;
}