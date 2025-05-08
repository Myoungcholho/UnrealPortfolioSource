#include "../Character/CAnimInstance.h"
#include "../Global.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Portfolio/Components/CMoveComponent.h"
#include "CPlayer.h"
#include "../Components/CMoveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Portfolio/Components/CLockonComponent.h"
#include "Portfolio/Components/CWeaponComponent.h"
#include "Portfolio/Weapons/SubActions/CSubAction.h"


void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if(OwnerCharacter == nullptr) return;

	MoveComponent = OwnerCharacter->FindComponentByClass<UCMoveComponent>();
	Player = Cast<ACPlayer>(OwnerCharacter);

	Weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(Weapon);
	
	Weapon->OnWeaponTypeChanged.AddDynamic(this,&UCAnimInstance::OnWeaponTypeChanged);

	LockOn = OwnerCharacter->FindComponentByClass<UCLockonComponent>();
	CheckNull(LockOn);
	
	Feet = FHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(OwnerCharacter == nullptr) return;

	// 캐릭터 Z 속도값
	VelocityZ = OwnerCharacter->GetVelocity().Z;
	
	// SubAction
	UCSubAction* subAction = Weapon->GetSubAction();
	if(!!subAction)
		bSubAction = subAction->GetInAction();
	
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator target = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, target, DeltaSeconds, 25);

	// StopMoving Direction용
	SlowPrevRotation = UKismetMathLibrary::RInterpTo(SlowPrevRotation, target, DeltaSeconds, 10);
	
	Direction = PrevRotation.Yaw;

	Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;
	
	Speed = OwnerCharacter->GetVelocity().Size2D();
	PrevSpeed = UKismetMathLibrary::FInterpTo(PrevSpeed,Speed,DeltaSeconds,25);
	if (Speed > 500 && !bRunCheck) // bRunCheck가 false일 때만 실행
	{
		bRunCheck = true;
		bTimer = true;
		//UE_LOG(LogTemp, Warning, TEXT("Run Check Activated"));
	}

	if(Speed <500 && bTimer)
	{
		bTimer = false;
		GetWorld()->GetTimerManager().SetTimer(RunCheckTimerHandle,this,&UCAnimInstance::ResetRunCheck,0.2f,false);
	}

	if(MoveComponent->ForwardInputValue == 0 && MoveComponent->RightInputValue ==0 && Speed <=300 && bRunCheck)
	{
		LastDirection = SlowPrevRotation.Yaw;
		bRealCheck = true;
		bRunCheck = false;
	}

	if(!!Player)
	{
		bIsFalling = Player->GetCharacterMovement()->IsFalling();
	}
	
	//LockOn , 리펙토링 대상
	if(!!LockOn)
	{
		bLockOn = LockOn->GetLockOn();
	}
	
	bUseFootIK = false;
	if (!!Feet)
	{
		bUseFootIK = true;
		FeetData = Feet->GetData();
	}

	CheckNull(Weapon);
	if (!!Weapon->GetSubAction())
	{
		bInBow_Aiming = true;
		bInBow_Aiming &= WeaponType == EWeaponType::Bow;
		bInBow_Aiming &= Weapon->GetSubAction()->GetInAction();
	}
	
	
	
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

void UCAnimInstance::ResetRunCheck()
{
	bRunCheck = false;
	bRealCheck = false;
}
