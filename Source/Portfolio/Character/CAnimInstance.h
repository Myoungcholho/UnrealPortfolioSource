#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Portfolio/Components/CFeetComponent.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "CAnimInstance.generated.h"

UCLASS()
class PORTFOLIO_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon_Bow")
	bool bInBow_Aiming;
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class ACPlayer* Player;
	UPROPERTY()
	class UCWeaponComponent* Weapon;
	UPROPERTY()
	class UCMoveComponent* MoveComponent;
	UPROPERTY()
	class UCLockonComponent* LockOn;
	class UCFeetComponent* Feet;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	float PrevSpeed;				// BS_Unarmed 사용
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	float Direction;				// BS_Unarmed 사용

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	float LastDirection;			// Bs_UnarmedStop 사용
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	bool bRealCheck = false;		// bluneprint 사용, Stopmotion 전이 조건

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	bool bIsFalling = false;		// Jump 사용

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	bool bLockOn = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	float Speed;					// Bs_Unarmed사용

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	bool bSubAction = false;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	float VelocityZ;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	bool bUseFootIK;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FFeetData FeetData;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::Max;
	
protected:
	FRotator PrevRotation;
	FRotator SlowPrevRotation;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
	
private:
	bool bTimer = false;
	bool bRunCheck = false;

private:
	FTimerHandle RunCheckTimerHandle;		// 타이머 핸들
	float BeforelerpValue;
	
private:
	void ResetRunCheck();
};
