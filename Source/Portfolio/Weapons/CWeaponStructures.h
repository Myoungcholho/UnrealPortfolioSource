#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "UObject/NoExportTypes.h"
#include "../GameEnums/CStateEnum.h"
#include "CWeaponStructures.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Katana, Bow, Club, Max,
};

UENUM(BlueprintType)
enum class EAttackHitType : uint8
{
	ComboAttack ,
	CommandAttack ,
	SpecialAttack,
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Basic,				// 평지에서만 밀림
	AirBorn,			// 공중에서만 밀림
	ComboAir,			// 공중 콤보용 공격
	Special,			// 특수 공격
};

// 코드 중복있다 해결하자 Enemy.h에
UENUM(BlueprintType)
enum class EExecutionType : uint8
{
	Groggy,
	BackStab,
	LastExecution,
	Max,
};

UENUM(BlueprintType)
enum class EAttackDirection : uint8
{
	None,
	Left,
	Right,
	Max,
};

USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere)
	float EquipPlayRate = 1.0f;
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* UnEquipMontage;

	UPROPERTY(EditAnywhere)
	float UnEquipPlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	bool bCanMove = false;
};

USTRUCT()
struct FEvadeData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Forward_Montage;
	UPROPERTY(EditAnywhere)
	float Forward_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Left_Montage;
	UPROPERTY(EditAnywhere)
	float Left_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Right_Montage;
	UPROPERTY(EditAnywhere)
	float Right_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Backward_Montage;
	UPROPERTY(EditAnywhere)
	float Backward_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* GuardBreak_Montage;
	UPROPERTY(EditAnywhere)
	float GuardBreak_PlayRate = 1.0f;
};

// 공격 정보
USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;
	
	UPROPERTY(EditAnywhere)
	bool bFixedCamera;
	
	UPROPERTY(EditAnywhere)
	FString Command;

	UPROPERTY(EditAnywhere)
	EHitResponseType AttackResponseType;
public:
	void DoAction(class ACharacter* InOwner);
	bool MatchesCommand(const FString& InputCommand) const
	{
		return Command == InputCommand;
	}
};

USTRUCT()
struct FHitAnimationData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	EAttackHitType AttackHitType;
	
	UPROPERTY(EditAnywhere)
	int32 AttackIndex;
};

USTRUCT()
struct FAttackProperty
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	
	UPROPERTY(EditAnywhere)
	float Power;

	UPROPERTY(EditAnywhere)
	bool bIsExplosiveAttack;

	UPROPERTY(EditAnywhere)
	EAttackType AttackType;

	UPROPERTY(EditAnywhere)
	bool bIgnoreGuard;

	UPROPERTY(EditAnywhere)
	EAttackDirection Direction;
};

USTRUCT()
struct FLaunchData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float Launch;

	UPROPERTY(EditAnywhere)
	float VerticalLaunch;
	
	UPROPERTY(EditAnywhere)
	FVector2D AirLaunch;
};

USTRUCT()
struct FHitEffectData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float GravityScale;
	
	UPROPERTY(EditAnywhere)
	float StopTime;

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake;
};

// 피격 정보
USTRUCT()
struct FDoHitData
{
	GENERATED_BODY()

public:
////////////////////////////////////////
///Hit Animation Data
	UPROPERTY(EditAnywhere)
	FHitAnimationData HitAnimationData;
////////////////////////////////////////	
	UPROPERTY(EditAnywhere)
	FAttackProperty AttackProperty;
/////// LunachData /////////////////////
	UPROPERTY(EditAnywhere)
	FLaunchData LaunchData;
///////////////////////////////////////
	UPROPERTY(EditAnywhere)
	FHitEffectData HitEffectData;

public:
	void SendDamage(class ACharacter* InAttacker, AActor* InAttackerCauser, class ACharacter* InOther);
	//void PlayHitMotion(class ACharacter* InCharacter);
	void PlayHitStop(class ACharacter* InCharacter);
	void PlayEffect(class ACharacter* InCharacter, class ACharacter* InAttacker = nullptr, EWeaponType InType = EWeaponType::Max, FVector* InLocation = nullptr);
	void PlaySound(class ACharacter* InCharacter);
	void PlayCameraShake(class ACharacter* InCharacter);
};
///////////////////////////--Guard--///////////////////////////
USTRUCT()
struct FGuardActivateData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* BeginGuardMontage;

	UPROPERTY(EditAnywhere)
	float BeginGuard_PlayRate = 1.0f;
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EndGuardMontage;

	UPROPERTY(EditAnywhere)
	float EndGuard_PlayRate = 1.0f;

public:
	void PlayBeginGuard(ACharacter* InCharacter);
	void PlayEndGuard(ACharacter* InCharacter);
};

USTRUCT()
struct FGuardBrokenData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* GuardBrokenMontage;

	UPROPERTY(EditAnywhere)
	float BeginGuard_PlayRate = 1.0f;

public:
	void PlayBrokenMotion(class ACharacter* InCharacter);
};

USTRUCT()
struct FGuardData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* GuardActionMontage;

	UPROPERTY(EditAnywhere)
	float GuardAction_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float Launch;

	UPROPERTY(EditAnywhere)
	float StopTime;

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake;
	
public:
	void PlayGuardMotion(class ACharacter* InCharacter);
	void PlayHitStop(class ACharacter* InCharacter);
	void PlaySound(class ACharacter* InCharacter);
	void PlayCameraShake(class ACharacter* InCharacter);
};

USTRUCT()
struct FMontageData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* PlayMontage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;
	
};

USTRUCT()
struct FExecutionMontageData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AssassinMontage;

	UPROPERTY(EditAnywhere)
	float AssassinPlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* TargetMontage;

	UPROPERTY(EditAnywhere)
	float TargetPlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	FVector ExecutionOffset;

	UPROPERTY(EditAnywhere)
	bool bIsBackExecution = false;
};

USTRUCT()
struct FHitGuardMontage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* GuardHitMontage;

	UPROPERTY(EditAnywhere)
	float GuardHit_PlayRate = 1.0f;
};

USTRUCT()
struct FExecutionData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EExecutionType Type;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* ActionMontage;

	UPROPERTY(EditAnywhere)
	float ActionPlayRate = 1.0f;
	
	UPROPERTY(EditAnywhere)
	bool bIsBackExecution = false;
	
	UPROPERTY(EditAnywhere)
	FVector ExecutionOffset;
	
	UPROPERTY(EditAnywhere)
	bool bFixedCamera = true;

	UPROPERTY(EditAnywhere)
	bool bCanMove = false;
	
	UPROPERTY(EditAnywhere)
	float Distance = 100.0f;

	UPROPERTY(EditAnywhere)
	float Angle = 30.0f;

	UPROPERTY(EditAnywhere)
	float Damage = 50.0f;
};

USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FDoHitData* HitData;
};

UCLASS()
class PORTFOLIO_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
