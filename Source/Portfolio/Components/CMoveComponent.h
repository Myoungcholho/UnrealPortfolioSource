// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "CMoveComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk=0, Run=1, Sprint=2, Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PORTFOLIO_API UCMoveComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category="Montages")
	FEvadeData Evade_Montage;
	
public:	
	UCMoveComponent();

protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// 이동 처리 함수
public:
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);
	void OnWalk();
	void OnRun();
	void OnSprint();
	void Move();
	void Stop();
	void Jump();
	
// 마우스 회전 함수
public:
	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);
	
public:
	void EnableControlRotation();
	void DisableControlRotation();
	
public:
	void EvadeAction();
	
// 카메라 bool 제어
public:
	FORCEINLINE bool GetFixedCamera() {return bFixedCamera;}
	FORCEINLINE bool EnableFixedCamera() {return bFixedCamera = true;}
	FORCEINLINE bool DisableFixedCamera() {return bFixedCamera = false;}
	
private:
	class ACharacter* OwnerCharacter;

private:
	UPROPERTY(EditDefaultsOnly,Category ="Speed")
	float Speed[(int32)ESpeedType::Max] = {200,400,600};

private:
	UPROPERTY(EditDefaultsOnly,Category ="Camera")
	FVector2D PitchAngle = FVector2D(-40,+40);

	UPROPERTY(EditDefaultsOnly,Category ="Camera")
	float HorizontalLook = 45;

	UPROPERTY(EditDefaultsOnly,Category ="Camera")
	float VerticalLook = 45;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DashAtTarget")
	float DashToSpeed = 10000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "DashAtTarget")
	float DashExitDistance = 175.0f;

	UPROPERTY(EditDefaultsOnly, Category = "DashAtTarget")
	float RotationSpeed = 25.0f;
	
	UPROPERTY(EditAnywhere, Category="DashAtTarget")
	float FinishAngle = 1.0f;

	UPROPERTY(EditAnywhere, Category="DashAtTarget")
	bool bLocationMove = false;
	
	UPROPERTY(EditAnywhere, Category="DashAtTarget")
	float MoveSpeed = 1000.0f;
	
public:
	float ForwardInputValue;
	float RightInputValue;
	
private:
	bool bCanMove = true;
	bool bFixedCamera = false;
	
private:
	ESpeedType MoveType = ESpeedType::Max;

public:
	void SetSpeed(ESpeedType InType);

public:
	// 날아가려면 대상 설정 해야함.
	void SetDestination();
	void MoveToTarget();
	
private:
	// 대상으로 날아가기 위한 정보들
	FVector TargetLocation;
	class AActor* Candidate;
	bool bDash = false;

	void UpdateMoveToTargetVelocity(float InDeltaTime);
	void UpdateMoveToTargetLocation(float InDeltaTime);
	
public:
	// 부드러운 회전 시작
	void RequestEnableControlRotation();
	FORCEINLINE bool IsControlRotationPending(){ return bEnableControlRotationPending == true;}
	
private:
	// 대상으로 천천히 회전할 수 있도록
	bool bEnableControlRotationPending = false;
	float RotationInterpSpeed  = 5.0f;
	float RotationSnapAngle = 3.0f;
	void UpdateSmoothRotationToFocus(float InDeltaTime);
	
};