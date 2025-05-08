#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM(Blueprintable)
enum class EAIStateType : uint8
{
	Wait =0,Patrol, Equip, Approach, Action, Guard, Damaged, Avoid, Strafe, Dead, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged,EAIStateType,InPrevType,EAIStateType,InNewType);

UCLASS()
class PORTFOLIO_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	friend class ACEnemy_AI;
	friend class ACEnemy_AI_Melee;
	
public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard){Blackboard = InBlackboard;}
	
private:
	UPROPERTY(EditAnywhere, Category="Blackboard Key")
	FName AIStateTypeKey = "State";
	
	UPROPERTY(EditAnywhere, Category="Blackboard Key")
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category="Blackboard Key")
	FName PatrolLocationKey ="Patrol";

	UPROPERTY(EditAnywhere, Category="Blackboard Key")
	FName AvoidLocationKey ="Avoid";

	UPROPERTY(EditAnywhere, Category="Blackboard Key")
	FName LocationKey ="Location";

	UPROPERTY(EditAnywhere, Category="Blackboard Key")
	FName ActionTimeKey ="ActionTime";
	
public:	
	UCAIBehaviorComponent();

private:
	EAIStateType GetType();

public:
	bool IsWaitMode();
	bool IsEquipMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsDamagedMode();
	bool IsAvoidMode();
	bool IsStrafeMode();
	bool IsGuardMode();

	void SetWaitMode();
	void SetEquipMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetDamagedMode();
	void SetAvoidMode();
	void SetStrafeMode();
	void SetGuardMode();
	

public:
	class ACharacter* GetTarget();

public:
	FVector GetPatrolLocation();
	void SetPatrolLocation(const FVector& InLocation);
	
	FVector GetLocation();

	float GetActionTime();
	void SetActionTime();
private:
	void ChangeType(EAIStateType InType);
	
public:
	FAIStateTypeChanged OnAIStateTypeChanged;
	
private:
	class UBlackboardComponent* Blackboard;
	
};