#include "../Enemies/CEnemy_AI_Melee.h"
#include "../Global.h"

#include "CAIController.h"
#include "Components/WidgetComponent.h"
#include "Portfolio/Components/CAIBehaviorComponent.h"
#include "Portfolio/Components/CHitMotionComponent.h"
#include "Portfolio/Components/CStatusComponent.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Stamina/CHealthStatus.h"
#include "Portfolio/Stamina/CStunStatus.h"
#include "Portfolio/Weapons/SubActions/CSubAction.h"
#include "Portfolio/Weapons/SubActions/CSubAction_Katana.h"
#include "Portfolio/Widget/CEnemyWidget.h"

ACEnemy_AI_Melee::ACEnemy_AI_Melee()
{
	FHelpers::CreateComponent<UWidgetComponent>(this,&Widget,"Widget",GetMesh());
	
	FString WidgetPath = TEXT("/Game/Enemies/UI/WBP_CEnemyHealthState.WBP_CEnemyHealthState_C");
	WidgetClass = (StaticLoadClass(UUserWidget::StaticClass(), nullptr, *WidgetPath));
	if (!!WidgetClass)
		Widget->SetWidgetClass(WidgetClass);

	Widget->SetRelativeLocation(WidgetLocation);
	
}

void ACEnemy_AI_Melee::BeginPlay()
{
	Super::BeginPlay();

	// Enemy Stamina Widget
	UCEnemyWidget* EnemyWidget = Cast<UCEnemyWidget>(Widget->GetUserWidgetObject());
	if(!!EnemyWidget)
	{
		FName name = FName(GetOwner()->GetName());
		EnemyWidget->UpdateName(name);
		
		EnemyWidget->UpdateHealth(Stamina->GetHealthStatus()->GetCurrent(),Stamina->GetHealthStatus()->GetMax());
		EnemyWidget->UpdateState(State->GetCurrentType(),State->GetPrevType());
		EnemyWidget->UpdateStun(Stamina->GetStunStatus()->GetCurrent(),Stamina->GetStunStatus()->GetMax());
		
		ACAIController* controller = Cast<ACAIController>(GetController());
		CheckNull(controller);
		UCAIBehaviorComponent* behavior = controller->GetAIBehavior();
		CheckNull(behavior);
		EnemyWidget->UpdateAIState(behavior->GetType(),behavior->GetType());
		
		Stamina->GetHealthStatus()->OnHealthChanged.AddDynamic(EnemyWidget,&UCEnemyWidget::UpdateHealth);
		Stamina->GetStunStatus()->OnStunChanged.AddDynamic(EnemyWidget,&UCEnemyWidget::UpdateStun);
		
		State->OnStateTypeChanged.AddDynamic(EnemyWidget,&UCEnemyWidget::UpdateState);
		behavior->OnAIStateTypeChanged.AddDynamic(EnemyWidget,&UCEnemyWidget::UpdateAIState);
	}

	// Widget Hide
	HideHPBar();
}

void ACEnemy_AI_Melee::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckNull(GetWorld());

	if(GetWorld()->GetTimeSeconds() > HPBarTimer + 30.0f)
	{
		HideHPBar();
	}
}

float ACEnemy_AI_Melee::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                   class AController* EventInstigator, AActor* DamageCauser)
{
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());

	ShowHPBar();
	
	if(State->IsDeadMode())
		return 0;

	IITeam* team = Cast<IITeam>(EventInstigator->GetPawn());
	if(team->GetTeamID() == TeamID)
		return 0;
	
	if(State->IsNoneMode() == true)
	{
		UCSubAction* subAction = Weapon->GetSubAction();
		UCSubAction_Katana* subActionKatana = Cast<UCSubAction_Katana>(subAction);
		if(!!subActionKatana)
		{
			if(State->IsGuardParryMode() && IsInFront() == true)
			{
				UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
				behavior->SetGuardMode();
				
				subActionKatana->PlayGuard(DamageData.Attacker);
				return 0;
			}
			else
			{
				float randomValue = FMath::FRand();
				if(randomValue <= 0.1f)
				{
					UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
					behavior->SetGuardMode();
					
					subActionKatana->PlayGuard(DamageData.Attacker);
					return 0;
				}
			}
		}
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACEnemy_AI_Melee::HitLaunch(FDoHitData* InHitData)
{
	Super::HitLaunch(InHitData);

	if(Stamina->GetHealthStatus()->IsDead() || State->IsDeadMode())
		return;
	
	float XYLaunch = 0.0f;
	float ZLaunch = 0.0f;

	FVector start = GetActorLocation();
	FVector target = DamageData.Attacker->GetActorLocation();
	FVector direction = CalculateLaunchDirection(start, target, DamageData.Attacker->GetActorForwardVector() ,InHitData->AttackProperty.bIsExplosiveAttack);

	// AirBorn 공격 처리
	if (InHitData->AttackProperty.AttackType == EAttackType::AirBorn)
	{
		XYLaunch = InHitData->LaunchData.Launch;
		ZLaunch = InHitData->LaunchData.VerticalLaunch;

		SetTargetRotation(start, target);
		ApplyLaunchForce(direction, XYLaunch, ZLaunch);
		return;
	}

	// 공중 상태에서의 Launch 처리
	if (State->IsAirborneMode())
	{
		XYLaunch = InHitData->LaunchData.AirLaunch.X;
		ZLaunch = InHitData->LaunchData.AirLaunch.Y;

		SetTargetRotation(start, target);
		ApplyLaunchForce(direction, XYLaunch, ZLaunch);
		return;
	}

	// 기본 Launch 처리
	if (InHitData->LaunchData.Launch > 0.0f)
	{
		SetTargetRotation(start, target);
		direction.Z = 0.0f;
		direction.Normalize();
		ApplyLaunchForce(direction, InHitData->LaunchData.Launch, 0.0f);
	}
}

bool ACEnemy_AI_Melee::CanBeAssassinated(EWeaponType InType,bool IsBack)
{
	if(State->IsDeadMode())
		return false;

	if(InType == EWeaponType::Katana)
		return true;
	

	if(InType == EWeaponType::Bow)
		return true;
	
	return false;
}

// 암살 당했을 때 Manager에 의해 호출
void ACEnemy_AI_Melee::OnAssassinated(ACharacter* Assassin, FExecutionData& InData, EWeaponType InType)
{
	if(InData.bIsBackExecution == true)
	{
		FVector AdjustedOffset = Assassin->GetActorForwardVector() * InData.ExecutionOffset.X +
							 Assassin->GetActorRightVector() * InData.ExecutionOffset.Y +
							 FVector(0.0f, 0.0f, InData.ExecutionOffset.Z);
	
		FVector NewLocation = Assassin->GetActorLocation() + AdjustedOffset;
		SetActorLocation(NewLocation);
		SetActorRotation(Assassin->GetActorRotation());
	}

	if(InData.bIsBackExecution == false)
	{
		FVector AssassinLocation = Assassin->GetActorLocation();
		FVector AdjustedOffset = Assassin->GetActorForwardVector() * InData.ExecutionOffset.X +
								 Assassin->GetActorRightVector() * InData.ExecutionOffset.Y +
								 FVector(0.0f, 0.0f, InData.ExecutionOffset.Z);
		
		FVector NewLocation = AssassinLocation + AdjustedOffset;
		SetActorLocation(NewLocation);
		
		FRotator NewRotation = Assassin->GetActorRotation() + FRotator(0.0f, 180.0f, 0.0f);
		SetActorRotation(NewRotation);
	}

	ShowHPBar();
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

UCWeaponComponent* ACEnemy_AI_Melee::GetWeaponComponent()
{
	CheckNullResult(Weapon,nullptr);

	return Weapon;
}

void ACEnemy_AI_Melee::ShowHPBar()
{
	UUserWidget* widget = Cast<UUserWidget>(Widget->GetWidget());
	CheckNull(widget);
	widget->SetVisibility(ESlateVisibility::Visible);

	ResetHideTimer();
}

void ACEnemy_AI_Melee::HideHPBar()
{
	UUserWidget* widget = Cast<UUserWidget>(Widget->GetWidget());
	CheckNull(widget);
	widget->SetVisibility(ESlateVisibility::Hidden);
}

void ACEnemy_AI_Melee::ResetHideTimer()
{
	HPBarTimer = GetWorld()->GetTimeSeconds();
}
