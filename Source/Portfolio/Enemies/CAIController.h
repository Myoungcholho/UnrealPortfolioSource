#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class PORTFOLIO_API ACAIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Percetion;

	UPROPERTY(VisibleAnywhere)
	class UCAIBehaviorComponent* Behavior;

public:
	FORCEINLINE UCAIBehaviorComponent* GetAIBehavior(){return Behavior;}
	
public:
	ACAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
private:
	FTimerHandle TargetClearTimerHandle;
	
	UFUNCTION()
	void OnPerceitonUpdate(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void ClearTarget();

private:
	UFUNCTION()
	void SubStateChanged(ESubStateType InPrevType, ESubStateType InNewType);
	
private:
	class ACEnemy_AI* Enemy;
	class UAISenseConfig_Sight* Sight;

public:
	bool bUpdatePercetion = true;
};
