#include "../Character/CPlayer.h"
#include "../Global.h"

#include "CAnimInstance.h"
#include "IGuardCount.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"

#include "../Components/CMoveComponent.h"
#include "../Components/CWeaponComponent.h"
#include "../Components/CStateComponent.h"
#include "Blueprint/UserWidget.h"
#include "Portfolio/Assassination/CAssassinationManager.h"
#include "Portfolio/Components/CCameraManagerComponent.h"
#include "Portfolio/Components/CCommandQueueComponent.h"
#include "Portfolio/Components/CDetectionComponent.h"
#include "Portfolio/Components/CLockonComponent.h"
#include "Portfolio/Components/CStatusComponent.h"
#include "Portfolio/Stamina/CHealthStatus.h"
#include "Portfolio/Weapons/DoActions/CDoAction.h"
#include "Portfolio/Weapons/SubActions/CDataAsset_Katana_Guard.h"
#include "Portfolio/Weapons/SubActions/CSubAction_Katana.h"
#include "Portfolio/Widget/CHUD.h"
#include "Portfolio/Widget/CStateWidget.h"
#include "../Components/CInventoryComponent.h"
#include "../Components/CInteractionComponent.h"
#include "Portfolio/Components/CHitMotionComponent.h"
#include "Portfolio/Components/CParkourComponent.h"
#include "Portfolio/Components/CQuickSlotComponent.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh
	USkeletalMesh* mesh;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Fallen_Samurai/Mesh/SK_Fallen_Samurai.SK_Fallen_Samurai'"));
	mesh = asset.Object;	
	
	
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));

	// Actor
	FHelpers::CreateComponent<USpringArmComponent>(this,&SpringArm,"SpringArm",GetCapsuleComponent());
	FHelpers::CreateComponent<UCameraComponent>(this,&Camera,"Camera",SpringArm);
	FHelpers::CreateComponent<UNiagaraComponent>(this,&HeadEffect,"HeadEffect",GetCapsuleComponent());

	CreateArrowGroup();
	
	// ActorComponent
	FHelpers::CreateActorComponent<UCWeaponComponent>(this,&Weapon,"Weapon");
	FHelpers::CreateActorComponent<UCStateComponent>(this,&State,"State");
	FHelpers::CreateActorComponent<UCLockonComponent>(this,&LockOn,"LockOn");
	FHelpers::CreateActorComponent<UCMoveComponent>(this,&Movement,"Movement");
	FHelpers::CreateActorComponent<UCStatusComponent>(this,&Status,"Status");
	FHelpers::CreateActorComponent<UCCommandQueueComponent>(this,&CommandQueue,"CommandQueue");
	
	FHelpers::CreateActorComponent<UCCameraManagerComponent>(this,&CamHandler3,"CamHandler3");
	FHelpers::CreateActorComponent<UCFeetComponent>(this,&FeetIK,"FeetIK");
	FHelpers::CreateActorComponent<UCDetectionComponent>(this,&Detection,"Detection");	
	FHelpers::CreateActorComponent<UCInventoryComponent>(this,&Inventory,"Inventory");
  	FHelpers::CreateActorComponent<UCInteractionComponent>(this,&Interaction,"Interaction");
	FHelpers::CreateActorComponent<UCQuickSlotComponent>(this,&QuickSlot,"QuickSlot");
	FHelpers::CreateActorComponent<UCHitMotionComponent>(this,&HitMotion,"HitMotion");
	FHelpers::CreateActorComponent<UCParkourComponent>(this,&Parkour,"Parkour");
	
	// Animation
	TSubclassOf<UCAnimInstance> animInstance;
	ConstructorHelpers::FClassFinder<UCAnimInstance> animAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/ABP_Samurai_Fallen.ABP_Samurai_Fallen_C'"));
	animInstance = animAsset.Class;
	
	GetMesh()->SetAnimClass(animInstance);
	
	// Camera
	GetCharacterMovement()->RotationRate = FRotator(0,720,0);
	SpringArm->SetRelativeLocation(FVector(0,0,60));
	SpringArm->TargetArmLength = 400;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	Tags.Add("Player");
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		ACHUD* hud = Cast<ACHUD>(PlayerController->GetHUD());
		hud->InitOverlay(State,Weapon,Inventory,QuickSlot,Status);
	}
	
	// 마우스 회전 Block
	Movement->DisableControlRotation();
	Movement->SetSpeed(ESpeedType::Run);
	// 캐릭터 속도 조정

	State->OnStateTypeChanged.AddDynamic(this,&ACPlayer::OnStateTypeChanged);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// BindAxis - 비 const 멤버 함수를 호출하도록 정의되어 있다.
	PlayerInputComponent->BindAxis("MoveForward",Movement,&UCMoveComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight",Movement,&UCMoveComponent::OnMoveRight);
	
	// Run
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,Movement,&UCMoveComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint",IE_Released,Movement,&UCMoveComponent::OnRun);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMoveComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMoveComponent::OnVerticalLook);
	PlayerInputComponent->BindAction("Jumpping",IE_Pressed,Movement,&UCMoveComponent::Jump);
	PlayerInputComponent->BindAction("Evade",IE_Pressed,Movement,&UCMoveComponent::EvadeAction);
	
	// Weapon
	PlayerInputComponent->BindAction("Katana",IE_Pressed,Weapon,&UCWeaponComponent::SetKatanaMode);
	PlayerInputComponent->BindAction("Bow",IE_Pressed,Weapon,&UCWeaponComponent::SetBowMode);

	// LockOn
	PlayerInputComponent->BindAction("LockOn",IE_Pressed,LockOn,&UCLockonComponent::ChangeLockMode);
	
	// SubAction(우클릭)
	PlayerInputComponent->BindAction("SubAction",IE_Pressed,Weapon,&UCWeaponComponent::SubAction_Pressed);
	PlayerInputComponent->BindAction("SubAction",IE_Released,Weapon,&UCWeaponComponent::SubAction_Released);

	// CommandQueue
	PlayerInputComponent->BindAction("Left",IE_Pressed,CommandQueue,&UCCommandQueueComponent::LeftKeyDown);
	PlayerInputComponent->BindAction("Right",IE_Pressed,CommandQueue,&UCCommandQueueComponent::RightKeyDown);
	PlayerInputComponent->BindAction("Up",IE_Pressed,CommandQueue,&UCCommandQueueComponent::UpKeyDown);
	PlayerInputComponent->BindAction("Down",IE_Pressed,CommandQueue,&UCCommandQueueComponent::DownKeyDown);
	PlayerInputComponent->BindAction("Action",EInputEvent::IE_Pressed, CommandQueue, &UCCommandQueueComponent::ProcessCommands);

	// Execution
	PlayerInputComponent->BindAction("Execution",EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAssassation);

	// CameraTest
	PlayerInputComponent->BindAction("CameraChange",EInputEvent::IE_Pressed, CamHandler3, &UCCameraManagerComponent::ChangeCamera);

	// Inventory & Interaction
	PlayerInputComponent->BindAction("Inventory",EInputEvent::IE_Pressed, Inventory, &UCInventoryComponent::ToggleInventory);
	PlayerInputComponent->BindAction("Interaction",EInputEvent::IE_Pressed, Interaction, &UCInteractionComponent::ExecuteInteraction);

	// QuickSlot
	PlayerInputComponent->BindAction("QuickSlotZ",EInputEvent::IE_Pressed, QuickSlot, &UCQuickSlotComponent::PressQuickSlotZ);
	PlayerInputComponent->BindAction("QuickSlotX",EInputEvent::IE_Pressed, QuickSlot, &UCQuickSlotComponent::PressQuickSlotX);

	// Parkour
	PlayerInputComponent->BindAction("DoParkour",EInputEvent::IE_Pressed,this, &ACPlayer::DoParkourAction);
}


float ACPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	IITeam* team = Cast<IITeam>(EventInstigator->GetPawn());
	if(team->GetTeamID() == TeamID)
		return 0;
	
	DamageData.Power = DamageAmount;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	if((DamageData.Event->HitData->AttackProperty.AttackType == EAttackType::Special) == false)
	{
		if((State->IsGuardingMode() || State->IsGuardParryMode()) && IsInFront() == true)
		{
			Guarded();
			return DamageAmount;
		}
	}
	Damaged();
	
	return DamageAmount;
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch(InNewType)
	{
	case EStateType::Damaged:
		ResetStatesOnDamage();
		break;
	case EStateType::Dead:
		break;
	}
}

void ACPlayer::ResetStatesOnDamage()
{
	UCSubAction* subAction = Weapon->GetSubAction();
	CheckNull(subAction);

	subAction->Released();
}

void ACPlayer::Damaged()
{
	if(DamageData.Attacker == GetOwner())
	{
		DamageData.Attacker = nullptr;
		DamageData.Causer = nullptr;
		DamageData.Event = nullptr;
		return;
	}
	
	State->SetDamagedMode();

	// HP 차감
	{
		//HealthPoint->Damage(DamageData.Power);
		Status->GetHealthStatus()->Decrease(DamageData.Power);
		DamageData.Power = 0;
	}

	EWeaponType weaponType = DamageData.Event->HitData->AttackProperty.WeaponType;
	EMonsterType monsterType = EMonsterType::Normal;
	
	// 데미지 피격 함수
	if(!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FDoHitData* hitData = DamageData.Event->HitData;

		//hitData->PlayHitMotion(this);
		HitMotion->HitReaction(DamageData.Attacker,DamageData.Event->HitData,weaponType,monsterType);
		hitData->PlayHitStop(this);
		
		//hitData->PlayEffect(this);
		//hitData->PlaySound(this);
		//hitData->PlayCameraShake(this);

		// Launch
		//if(HealthPoint->IsDead() == false)
		if(Status->GetHealthStatus()->IsDead() == false)
		{
			FVector start = GetActorLocation();
			FVector target = DamageData.Attacker->GetActorLocation();
			FVector direction;

			// 방향 설정
			if(hitData->AttackProperty.bIsExplosiveAttack == false)
				direction = -(DamageData.Attacker->GetActorForwardVector());
			else
				direction = target - start;
			
			direction.Normalize();
			
			if(hitData->LaunchData.Launch > 0.0f)
			{
				LaunchCharacter(-direction* hitData->LaunchData.Launch,false,false);
				// 
			}

			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start,target);
			rotator.Pitch = 0.0f;
			rotator.Roll = 0.0f;
			SetActorRotation(rotator);	
		}
	}
	
	//if(HealthPoint->IsDead())
	if(Status->GetHealthStatus()->IsDead())
	{
		State->SetDeadMode();

		return;
	}
	
	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;
	DamageData.Event = nullptr;
}

void ACPlayer::Guarded()
{
	UCSubAction* subAction = Weapon->GetSubAction();
	UCSubAction_Katana* subActionKatana = Cast<UCSubAction_Katana>(subAction);
	CheckNull(subActionKatana);
	
	if(State->IsJustGuardMode())
	{
		IIGuardCount* target = Cast<IIGuardCount>(DamageData.Attacker);
		target->ReceiveGuardCounter();
	}
	
	subActionKatana->PlayGuard(DamageData.Attacker);

	// 만약 맞는데 문제생기면
	// DataDamage 값 초기화할것.
}

bool ACPlayer::IsInFront()
{
	if(DamageData.Attacker == nullptr)
		return false;
	
	FVector ToAttacker = DamageData.Attacker->GetActorLocation() - GetActorLocation();
	ToAttacker.Normalize();
	
	float DotProduct = FVector::DotProduct(GetActorForwardVector(), ToAttacker);
	
	return DotProduct > 0;
}

void ACPlayer::OnMoveTypeChanged(ESpeedType InMoveType)
{
	MoveType = InMoveType;
}

UCWeaponComponent* ACPlayer::GetWeaponComponent()
{
	CheckNullResult(Weapon,nullptr);
	
	return Weapon;
}



void ACPlayer::CreateArrowGroup()
{
	FHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "Arrows", GetCapsuleComponent());

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		FHelpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name), ArrowGroup);

		switch ((EParkourArrowType)i)
		{
		case EParkourArrowType::Center:
			Arrows[i]->ArrowColor = FColor::Red;
			break;

		case EParkourArrowType::Head:
			Arrows[i]->ArrowColor = FColor::Green;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, 100));
			break;

		case EParkourArrowType::Foot:
			Arrows[i]->ArrowColor = FColor::Blue;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, -80));
			break;

		case EParkourArrowType::Left:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, -30, 0));
			break;

		case EParkourArrowType::Right:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, +30, 0));
			break;

		case EParkourArrowType::Land:
			Arrows[i]->ArrowColor = FColor::Yellow;
			Arrows[i]->SetRelativeLocation(FVector(50, 0, 100));
			Arrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
			break;
		}
	}
}

void ACPlayer::DoParkourAction()
{
	if (!!Parkour)
	{
		Parkour->DoParkour();

		return;
	}
}

void ACPlayer::SetTeamID(uint8 InTeamID)
{
	TeamID = InTeamID;
}

uint8 ACPlayer::GetTeamID()
{
	return TeamID;
}

void ACPlayer::End_Damaged()
{
	State->SetIdleMode();

	UCDoAction* doAction = Weapon->GetDoAction();
	CheckNull(doAction);

	doAction->End_DoAction();
	Weapon->SubAction_Released();
}

void ACPlayer::End_Evade()
{
	State->SetIdleMode();
}

void ACPlayer::End_Assassination()
{
	State->SetIdleMode();

	if(!!Movement)
	{
		Movement->DisableFixedCamera();
		Movement->Move();
	}
}

bool ACPlayer::CanBeAssassinated(EWeaponType InType,bool IsBack)
{
	if(State->IsDeadMode())
		return false;
	
	return true;
}

// 암살 실행 몽타주
void ACPlayer::ExecuteAssassination(ACharacter* Target,FExecutionData& InData, EWeaponType InType)
{
	State->SetAssassinationMode();
	
	if(!!Movement)
	{
		if(InData.bFixedCamera)
			Movement->EnableFixedCamera();
		if(InData.bCanMove == false)
			Movement->Stop();
	}
	
	if(!!InData.ActionMontage)
	{
		PlayAnimMontage(InData.ActionMontage,InData.ActionPlayRate);

		// 카메라 전환 타입 0일때,일단 임시로
		if(InData.Type == EExecutionType::BackStab)
			CamHandler3->ChangeToSubCamera("AssassinationCamera");

		if(InData.Type == EExecutionType::Groggy)
			CamHandler3->ChangeToSubCamera("GroggyCamera");

		if(InData.Type == EExecutionType::LastExecution)
			CamHandler3->ChangeToSubCamera("LastExecution");
	}
}