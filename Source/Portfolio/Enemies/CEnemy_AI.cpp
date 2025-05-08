#include "../Enemies/CEnemy_AI.h"

#include "CAIController.h"
#include "../Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Portfolio/Components/CAIBehaviorComponent.h"
#include "Portfolio/Components/CStateComponent.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Widget/CEnemyWidget.h"
#include "Components/WidgetComponent.h"
#include "Portfolio/Components/CHitMotionComponent.h"
#include "Portfolio/Components/CStatusComponent.h"
#include "Portfolio/Managers/CGameInstance.h"
#include "Portfolio/Managers/CTeamCombatManager.h"
#include "Portfolio/Stamina/CHealthStatus.h"
#include "Portfolio/Stamina/CStunStatus.h"
#include "Portfolio/Weapons/SubActions/CSubAction_Katana.h"
#include "Portfolio/Widget/CHeartWidget.h"

ACEnemy_AI::ACEnemy_AI()
{
	FHelpers::CreateActorComponent<UCWeaponComponent>(this,&Weapon,"WeaponComponent");
	FHelpers::CreateComponent<UWidgetComponent>(this,&HeartWidget,"HeartWidget",GetMesh());
	FHelpers::CreateActorComponent<UCVisualEffectComponent>(this,&VisualEffect,"VisualEffect");
	
	FString HeartWidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Enemies/UI/WBP_CHeartTarget.WBP_CHeartTarget_C'");
	HeartWidgetClass = (StaticLoadClass(UUserWidget::StaticClass(), nullptr, *HeartWidgetPath));
	if (!!HeartWidgetClass)
		HeartWidget->SetWidgetClass(HeartWidgetClass);

	HeartWidget->SetRelativeLocation(HeartWidgetLocation);
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();
	
	// 적 그로기 표시 위젯
	UCHeartWidget* heartWidget = Cast<UCHeartWidget>(HeartWidget->GetUserWidgetObject());
	if(!!heartWidget)
	{
		State->OnStatusEffectChanged.AddDynamic(heartWidget,&UCHeartWidget::ToggleWidgetVisibility);
		heartWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	// 본인 TeamManager에 등록
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetOwner()->GetGameInstance());
	CheckNull(gameInstance);

	UCTeamCombatManager* teamCombatManager = gameInstance->GetTeamCombatManager();
	CheckNull(teamCombatManager);

	// 등록
	teamCombatManager->RegisterActor(GetOwner(),TeamID);
}

void ACEnemy_AI::Damaged()
{
	Super::Damaged();
	
	CheckTrue(State->IsDeadMode());
	CheckTrue(State->IsSuperArmorMode())
	
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
	behavior->SetDamagedMode();
}

void ACEnemy_AI::Dead()
{
	Super::Dead();

	ACAIController* controller = Cast<ACAIController>(GetController());
	CheckNull(controller);
	// 중지하고 이유를 기록 ["is dead"]
	controller->BrainComponent->StopLogic(TEXT("is dead"));
	controller->bUpdatePercetion = false;
}

void ACEnemy_AI::Idle()
{
	Super::Idle();
	CheckNull(GetController());
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());

	CheckNull(behavior);
	behavior->SetWaitMode();
}

void ACEnemy_AI::End_DoAction()
{
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
	behavior->SetWaitMode();
}

bool ACEnemy_AI::IsInFront()
{
	if(DamageData.Attacker == nullptr)
		return false;
	
	FVector ToAttacker = DamageData.Attacker->GetActorLocation() - GetActorLocation();
	ToAttacker.Normalize();
	
	float DotProduct = FVector::DotProduct(GetActorForwardVector(), ToAttacker);
	
	return DotProduct > 0;
}

void ACEnemy_AI::End_Damaged()
{
	Super::End_Damaged();

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
	behavior->SetWaitMode();
}

void ACEnemy_AI::End_GuardImpact()
{
	Super::End_GuardImpact();

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(GetController());
	behavior->SetWaitMode();
}
