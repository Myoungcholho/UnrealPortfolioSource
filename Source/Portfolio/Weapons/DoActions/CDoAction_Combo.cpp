#include "../../Weapons/DoActions/CDoAction_Combo.h"
#include "../../Global.h"

#include "GameFramework/Character.h"
#include "../../Components/CStateComponent.h"
#include "../../Components/CMoveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCDoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() <1);
	if(bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}
	
	CheckFalse(State->IsIdleMode());
	Super::DoAction();
	//FLog::Log("call Montage DoAction");
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	if(State->IsDamagedMode())
		return;
	if(State->IsGuardParryMode())
		return;
	if(State->IsGuardBreakMode())
		return;
	if(State->IsActionMode() == false)
		return;
	
	if(Index + 1 >= DoActionDatas.Num())
		return;
	
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	//FLog::Log("call Montage BeginDoAction");
	
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();
	
	bExist = false;
	Index = 0;
	AirIndex = -1;
	OwnerCharacter->GetCharacterMovement()->GravityScale = 1.0f;
}

void UCDoAction_Combo::Begin_Air_DoAction()
{
	if(State->IsDamagedMode())
		return;
	if(State->IsGuardParryMode())
		return;
	if(State->IsGuardBreakMode())
		return;
	if(State->IsActionMode() == false)
		return;

	if(AirIndex > DoAirActionDatas.Num()-1)
		return;

	CheckFalse(bExist);
	Super::Begin_Air_DoAction();

	bExist = false;
	
	OwnerCharacter->GetCharacterMovement()->GravityScale = 0.25f;
	DoAirActionDatas[++AirIndex].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	CheckNull(Candidate);

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(controller);
	
	FRotator controlRotation = OwnerCharacter->GetControlRotation();
	FRotator ownerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(),Candidate->GetActorLocation());

	ownerToTarget.Pitch = controlRotation.Pitch;

	
	if(controlRotation.Equals(ownerToTarget,FinishAngle))
	{
		Candidate = nullptr;
		controller->SetControlRotation(ownerToTarget);

		return;
	}
	
	FRotator targetRotation = FRotator(controlRotation.Pitch, ownerToTarget.Yaw,ownerToTarget.Roll);
	FRotator result = UKismetMathLibrary::RInterpTo(controlRotation,targetRotation,InDeltaTime,RotationSpeed);

	controller->SetControlRotation(result);
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser,
	class ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);

	for(ACharacter* hitted: Hitted)
	{
		CheckTrue(hitted == InOther);
	}

	Hitted.AddUnique(InOther);

	if(bAirAction == true)
	{
		CheckTrue(HitAirDatas.Num()-1 < AirIndex);
		HitAirDatas[AirIndex].SendDamage(InAttacker,InAttackCauser,InOther);
		//FLog::Log("bAirAction Hit SendDamage");
		return;
	}
	
	if(bCommandAction == true)
	{
		CheckTrue(HitCommandDatas.Num()-1<CommandIndex);
		HitCommandDatas[CommandIndex].SendDamage(InAttacker,InAttackCauser,InOther);
		//FLog::Log("bCommandAction Hit SendDamage");
		return;
	}
	
	CheckTrue(HitDatas.Num()-1	<Index);
	HitDatas[Index].SendDamage(InAttacker,InAttackCauser,InOther);
}

// 충돌이 끝난경우 후보를 선택,
// 후보자 등록,
// Tick에서 대상으로 회전
void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();

	if(Movement->GetFixedCamera() == false)
	{
		Hitted.Empty();

		return;
	}
	
	float angle = -2.0f;

	for(ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		FVector forward = OwnerCharacter->GetActorForwardVector();

		float dot = FVector::DotProduct(direction, forward);
		if(dot <0.75f || dot <angle)
			continue;
		
		angle = dot;
		Candidate = hitted;
	}
	
	Hitted.Empty();
	//FLog::Log("End Collision");
}
