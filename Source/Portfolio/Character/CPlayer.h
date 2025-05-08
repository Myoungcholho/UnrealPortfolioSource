#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IStatable.h"
#include "ITeam.h"
#include "IWeaponUser.h"
#include "Portfolio/Assassination/IAssassination.h"
#include "Portfolio/Components/CStateComponent.h"
#include "Portfolio/GameEnums/CParkourEnum.h"
#include "Portfolio/Weapons/CWeaponStructures.h"
#include "CPlayer.generated.h"


enum class ESpeedType : uint8;

UCLASS()
class PORTFOLIO_API ACPlayer : public ACharacter 
	, public IIWeaponUser ,public IIStatable, public IITeam, public IIAssassination
{
	GENERATED_BODY()

public:
	ACPlayer();

private:
	UPROPERTY(EditDefaultsOnly, Category= "Team")
	uint8 TeamID = 1;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	UPROPERTY(EditDefaultsOnly,Category="UesrInterface")
	TSubclassOf<class UCStateWidget> StateWidgetClass;

// Actor
private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* HeadEffect;
	
// ActorComponent
private:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;
	
	UPROPERTY(VisibleAnywhere)
	class UCMoveComponent* Movement;

	UPROPERTY(VisibleAnywhere)
	class UCLockonComponent* LockOn;

	UPROPERTY(VisibleAnywhere)
	class UCStatusComponent* Status;
	
	UPROPERTY(VisibleAnywhere)
	class UCCameraManagerComponent* CamHandler3;
	
	UPROPERTY(VisibleAnywhere)
	class UCFeetComponent* FeetIK;

	UPROPERTY(VisibleAnywhere)
	class UCDetectionComponent* Detection;
	
	UPROPERTY(VisibleAnywhere)
	class UCCommandQueueComponent* CommandQueue;

	UPROPERTY(VisibleAnywhere)
	class UCInventoryComponent* Inventory;

	UPROPERTY(VisibleAnywhere)
	class UCInteractionComponent* Interaction;

	UPROPERTY(VisibleAnywhere)
	class UCQuickSlotComponent* QuickSlot;

	UPROPERTY(VisibleAnywhere)
	class UCHitMotionComponent* HitMotion;

private:
	UPROPERTY(VisibleAnywhere)
	class UCParkourComponent* Parkour;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* ArrowGroup;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];
	
	
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	void ResetStatesOnDamage();
protected:
	virtual void Damaged();
	void Guarded();

private:
	bool IsInFront();
	
public:
	FORCEINLINE ESpeedType& GetMoveType(){return MoveType;} 
	
private:
	ESpeedType MoveType;

private:
	FRotator PrevRotation;
	float Direction;
	
private:
	UFUNCTION()
	void OnMoveTypeChanged(ESpeedType InMoveType);

public:
	virtual UCWeaponComponent* GetWeaponComponent() override;

private:
	void CreateArrowGroup();
	void DoParkourAction();
	
//interface
public:
	virtual void SetTeamID(uint8 InTeamID) override;
	virtual uint8 GetTeamID() override;
	
//IStatable
	virtual void End_Damaged() override;
	virtual void End_Evade() override;
	virtual void End_Assassination() override;

//IAssassination
	virtual bool CanBeAssassinated(EWeaponType InType,bool IsBack) override;
	virtual void ExecuteAssassination(ACharacter* Target, FExecutionData& InData, EWeaponType InType) override;

private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	};
	FDamageData DamageData;
};
