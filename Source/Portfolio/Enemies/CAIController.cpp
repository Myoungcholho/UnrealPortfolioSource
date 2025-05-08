#include "../Enemies/CAIController.h"

#include "CEnemy_AI.h"
#include "../Global.h"

#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Components/CAIBehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Portfolio/Components/CLockonComponent.h"
#include "Portfolio/Components/CStateComponent.h"
#include "Portfolio/Managers/CGameInstance.h"
#include "Portfolio/Managers/CTeamCombatManager.h"

ACAIController::ACAIController()
{
	FHelpers::CreateActorComponent<UAIPerceptionComponent>(this,&Percetion,"Perception");
	FHelpers::CreateActorComponent<UCAIBehaviorComponent>(this,&Behavior,"Behavior");
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 1000;
	Sight->LoseSightRadius = 1500;
	Sight->PeripheralVisionAngleDegrees = 55;
	Sight->SetMaxAge(2);			// 반경에 없을 시 잃을 시간

	// 적 감지 설정
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;	
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	
	Percetion->ConfigureSense(*Sight);	// 컴포넌트에 설정
	Percetion->SetDominantSense(*Sight->GetSenseImplementation());	// 감지 우선순위 설정
	
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Percetion->OnPerceptionUpdated.AddDynamic(this,&ACAIController::OnPerceitonUpdate);
	Enemy->OnBegin_Dead.AddDynamic(this,&ACAIController::ClearTarget);

	Enemy->State->OnSubStateTypeChanged.AddDynamic(this,&ACAIController::SubStateChanged);
}

void ACAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckNull(Behavior);
	
	// blackboard Target state update
	ACharacter* target = Behavior->GetTarget();
	if(!!target)	
	{
		// 만약 타겟이 죽으면 Clear 상태로
		UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(target);
		CheckNull(state);
		if(state->IsDeadMode())
			ClearTarget();
	}
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);
	CheckNull(Enemy);
	
	CheckNull(Enemy->GetBehaviorTree());
	RunBehaviorTree(Enemy->GetBehaviorTree());
	
	Behavior->SetBlackboard(Blackboard);
	
	this->bAllowStrafe = true;
}

void ACAIController::OnUnPossess()
{
	AActor* PrevTarget = Cast<AActor>(Blackboard->GetValueAsObject("Target"));
	
	if (PrevTarget)
	{
		UCGameInstance* instance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
		if(!!instance)
		{
			UCTeamCombatManager* manager = instance->GetTeamCombatManager();
			if(!!manager)
				manager->ReleaseApproach(Enemy->GetOwner(),PrevTarget);
		}
	}
	
	Super::OnUnPossess();
}

// Percetion될때마다 호출
void ACAIController::OnPerceitonUpdate(const TArray<AActor*>& UpdatedActors)
{
	if(bUpdatePercetion == false)
		return;

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(Enemy);
	CheckNull(state);

	if(state->IsDownMode())
		return;
	if(state->IsDeadMode())
		return;
	
	TArray<AActor*> actors;
	Percetion->GetCurrentlyPerceivedActors(nullptr,actors);

	for(AActor* actor : actors)
	{
		IITeam* team = Cast<IITeam>(actor);
		IITeam* owner = Cast<IITeam>(Enemy);
		
		if(team == nullptr) continue;
		if(owner == nullptr) continue;
		
		// 같은 팀이라면 건너 뜀
		if(team->GetTeamID() == owner->GetTeamID())
			continue;
		
		// 다른 팀이라면 등록 후 리턴
		CheckNull(Blackboard);
		
		ACharacter* character = Cast<ACharacter>(GetPawn());
		CheckNull(character);
		character->bUseControllerRotationYaw = true;
		character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Blackboard->SetValueAsObject("Target",actor);
		SetFocus(actor);

		UCLockonComponent* lockOn = FHelpers::GetComponent<UCLockonComponent>(Enemy);
		if(!!lockOn)
			lockOn->OnLock();
		
		Enemy->ShowHPBar();
		
		// 타이머 취소 (기존 초기화 타이머가 있으면 취소)
		GetWorld()->GetTimerManager().ClearTimer(TargetClearTimerHandle);

		return;
	}

	// 감지 대상이 없어진다면
	GetWorld()->GetTimerManager().SetTimer(
		TargetClearTimerHandle,
		this,
		&ACAIController::ClearTarget,
		4.0f, // 2초 동안 유지
		false // 반복하지 않음
	);
}

void ACAIController::ClearTarget()
{
	CheckNull(Enemy);
	CheckNull(Blackboard);
	
	AActor* PrevTarget = Cast<AActor>(Blackboard->GetValueAsObject("Target"));
	
	if (PrevTarget)
	{
		UCGameInstance* instance = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
		if(!!instance)
		{
			UCTeamCombatManager* manager = instance->GetTeamCombatManager();
			if(!!manager)
				manager->ReleaseApproach(Enemy->GetOwner(),PrevTarget);
		}
	}
	
	Blackboard->SetValueAsObject("Target", nullptr);
	ClearFocus(EAIFocusPriority::Gameplay);
	
	UCLockonComponent* lockOn = FHelpers::GetComponent<UCLockonComponent>(Enemy);
	if(!!lockOn)
		lockOn->OffLock();
}

void ACAIController::SubStateChanged(ESubStateType InPrevType, ESubStateType InNewType)
{
	if(InPrevType == ESubStateType::Down && InNewType == ESubStateType::None)
	{
		TArray<AActor*> perceivedActors;
		Percetion->GetCurrentlyPerceivedActors(nullptr, perceivedActors);

		for (AActor* actor : perceivedActors)
		{
			Percetion->ForgetActor(actor);
		}
		
		Percetion->RequestStimuliListenerUpdate();
	}
}
