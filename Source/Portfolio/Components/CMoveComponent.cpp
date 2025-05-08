#include "../Components/CMoveComponent.h"

#include "AIController.h"
#include "CLockonComponent.h"
#include "CStateComponent.h"
#include "CWeaponComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Portfolio/Character/CPlayer.h"
#include "Portfolio/Utilities/FHelpers.h"
#include "Portfolio/Utilities/FLog.h"


UCMoveComponent::UCMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(OwnerCharacter == nullptr) return;
	
	APlayerController* playerController = OwnerCharacter->GetController<APlayerController>();
	if(!!playerController)
	{
		playerController->PlayerCameraManager->ViewPitchMin = PitchAngle.X;
		playerController->PlayerCameraManager->ViewPitchMax = PitchAngle.Y;
	}
}

void UCMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateSmoothRotationToFocus(DeltaTime);
	
	CheckNull(Candidate);
	if (bDash == false)
		return;
	if(bLocationMove == false)
		UpdateMoveToTargetVelocity(DeltaTime);
	else
		UpdateMoveToTargetLocation(DeltaTime);
	
}

void UCMoveComponent::OnMoveForward(float InAxisValue)
{
	ForwardInputValue = InAxisValue;
	
	if(bCanMove == false)
		return;

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state)
	if(!!state)
	{
		if(state->IsDamagedMode())
			return;
	}
	
	FRotator rotator = FRotator(0,OwnerCharacter->GetControlRotation().Yaw,0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction,InAxisValue);
}

void UCMoveComponent::OnMoveRight(float InAxisValue)
{
	RightInputValue = InAxisValue;
	
	if(bCanMove == false)
		return;
		
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state)
	if(!!state)
	{
		if(state->IsDamagedMode())
			return;
	}
	
	FRotator rotator = FRotator(0,OwnerCharacter->GetControlRotation().Yaw,0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction,InAxisValue);
}

void UCMoveComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMoveComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMoveComponent::OnSprint()
{
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	if(!!state)
	{
		if(state->IsGuardingMode())
			return;
	}
	SetSpeed(ESpeedType::Sprint);
}

void UCMoveComponent::Move()
{
	bCanMove = true;
}

void UCMoveComponent::Stop()
{
	bCanMove = false;
}

void UCMoveComponent::Jump()
{
	OwnerCharacter->GetCharacterMovement()->JumpZVelocity = 750.f; // 높이 조정
	OwnerCharacter->Jump();
}

void UCMoveComponent::OnHorizontalLook(float InAxisValue)
{
	if(bFixedCamera == true) return;
	
	OwnerCharacter->AddControllerYawInput(InAxisValue * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCMoveComponent::OnVerticalLook(float InAxisValue)
{
	if(bFixedCamera == true) return;
	
	OwnerCharacter->AddControllerPitchInput(InAxisValue * VerticalLook * GetWorld()->GetDeltaSeconds());
}

void UCMoveComponent::SetSpeed(ESpeedType InType)
{
	MoveType = InType;
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int)InType];
}

void UCMoveComponent::SetDestination()
{
 	CheckNull(OwnerCharacter);
	UCLockonComponent* lockOn = FHelpers::GetComponent<UCLockonComponent>(OwnerCharacter);
	CheckNull(lockOn);

	Candidate = lockOn->GetTargetInFront();

	// 대상자 없다면 몽타주 취소
	if(Candidate == nullptr)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		CheckNull(AnimInstance);
		
		if (AnimInstance->Montage_IsPlaying(nullptr))
			AnimInstance->Montage_Stop(0.2f); 

		// 상태 전부 초기화
		IIStatable* statable = Cast<IIStatable>(OwnerCharacter);
		CheckNull(statable);
		statable->End_Damaged();
		
		return;
	}
	
	TargetLocation = Candidate->GetActorLocation();
}

void UCMoveComponent::MoveToTarget()
{
	CheckNull(Candidate);
	CheckNull(OwnerCharacter);
	
	FVector Direction = (Candidate->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();
	CheckNull(MovementComponent);

	MovementComponent->Velocity = Direction * DashToSpeed;
	bDash = true;
}

void UCMoveComponent::UpdateMoveToTargetVelocity(float InDeltaTime)
{
	float Distance = FVector::Dist(OwnerCharacter->GetActorLocation(),Candidate->GetActorLocation());
	if (Distance <= DashExitDistance)
	{
		UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();
		MovementComponent->StopMovementImmediately();
		bDash = false;
		Candidate = nullptr;
		return;
	}
	
	FVector Direction = (Candidate->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	
	OwnerCharacter->GetCharacterMovement()->Velocity = Direction * DashToSpeed;

	/////////////////////////
	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(controller);
	
	FRotator controlRotation = OwnerCharacter->GetControlRotation();
	FRotator ownerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(),Candidate->GetActorLocation());

	ownerToTarget.Pitch = controlRotation.Pitch;
		
	if(controlRotation.Equals(ownerToTarget,FinishAngle))
	{
		controller->SetControlRotation(ownerToTarget);
		return;
	}
	
	FRotator targetRotation = FRotator(controlRotation.Pitch, ownerToTarget.Yaw,ownerToTarget.Roll);
	FRotator result = UKismetMathLibrary::RInterpTo(controlRotation,targetRotation,InDeltaTime,RotationSpeed);

	controller->SetControlRotation(result);
}

void UCMoveComponent::UpdateMoveToTargetLocation(float InDeltaTime)
{
	
	// TargetLocation 으로 회전 없이 Lerp하여 SetActorLocation 실행
	const FVector currentLocation = OwnerCharacter->GetActorLocation();
	
	float Distance = FVector::Dist(currentLocation,TargetLocation);
	if (Distance <= DashExitDistance)
	{
		UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();
		MovementComponent->StopMovementImmediately();
		bDash = false;
		Candidate = nullptr;
		return;
	}

	const FVector direction = (TargetLocation- currentLocation).GetSafeNormal();
	const FVector NewLocation = currentLocation + direction * MoveSpeed * InDeltaTime;
	OwnerCharacter->SetActorLocation(NewLocation, true);
}

void UCMoveComponent::RequestEnableControlRotation()
{
	bEnableControlRotationPending = true;
}

void UCMoveComponent::UpdateSmoothRotationToFocus(float InDeltaTime)
{
	if (!bEnableControlRotationPending || OwnerCharacter == nullptr)
		return;

	AAIController* controller = Cast<AAIController>(OwnerCharacter->GetController());
	if (!controller) return;

	AActor* FocusTarget = controller->GetFocusActor();
	if (!FocusTarget) return;

	
	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), FocusTarget->GetActorLocation());
	FRotator SmoothRot = FMath::RInterpTo(OwnerCharacter->GetActorRotation(), TargetRot, InDeltaTime, RotationInterpSpeed);
	
	OwnerCharacter->SetActorRotation(SmoothRot);


	float YawDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(OwnerCharacter->GetActorRotation().Yaw, TargetRot.Yaw));
	//FLog::Log(YawDiff);
	if (YawDiff < RotationSnapAngle)
	{
		EnableControlRotation();
		bEnableControlRotationPending = false;
	}
}

// 움직이는 방향으로 회전 안함.
void UCMoveComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

// 움직이는 방향으로 회전
void UCMoveComponent::DisableControlRotation()
{
	bEnableControlRotationPending = false;
	// 컨트롤러가 회전해도 캐릭터는 영향을 받지 않음 (마우스)
	OwnerCharacter->bUseControllerRotationYaw = false;
	// 캐릭터는 이동 방향을 기준으로 자동으로 회전 (키보드)
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMoveComponent::EvadeAction()
{
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);

	// 0. 이미 evade라면 return;
	if(state->IsEvade())
		return;
	if(state->IsAssassinationMode())
		return;
	
	// 1. 공격/가드 상태 초기화
	IIStatable* statable = Cast<IIStatable>(OwnerCharacter);
	CheckNull(statable);
	statable->End_Damaged();

	// 3. 회피 방향 결정
	FVector2D inputDirection = FVector2D(ForwardInputValue, RightInputValue).GetSafeNormal();
	UAnimMontage* selectedMontage = nullptr;
	float playRate = 1.0f;

	if(FMath::IsNearlyZero(inputDirection.Size()))
		return;
	
	state->SetEvadeMode();
	
	// 4. 입력 방향에 따라 애니메이션 선택
	if (inputDirection.X > 0.5f)  // Forward
	{
		selectedMontage = Evade_Montage.Forward_Montage;
		playRate = Evade_Montage.Forward_PlayRate;
	}
	else if (inputDirection.X < -0.5f)  // Backward
	{
		selectedMontage = Evade_Montage.Backward_Montage;
		playRate = Evade_Montage.Backward_PlayRate;
	}
	else if (inputDirection.Y > 0.5f)  // Right
	{
		selectedMontage = Evade_Montage.Right_Montage;
		playRate = Evade_Montage.Right_PlayRate;
	}
	else if (inputDirection.Y < -0.5f)  // Left
	{
		selectedMontage = Evade_Montage.Left_Montage;
		playRate = Evade_Montage.Left_PlayRate;
	}

	// 5. 애니메이션 재생
	if (!!selectedMontage)
		OwnerCharacter->PlayAnimMontage(selectedMontage, playRate);
	
}
