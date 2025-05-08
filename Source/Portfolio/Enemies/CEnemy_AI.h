#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "../Character/CEnemy.h"
#include "Portfolio/Character/IWeaponUser.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class PORTFOLIO_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Widget")
	FVector HeartWidgetLocation = FVector(0,0,100);
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category="AI")
	class ACPatrolPath* PatrolPath;
protected:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HeartWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCHeartWidget> HeartWidgetClass;

	UPROPERTY(VisibleAnywhere)
	class UCVisualEffectComponent* VisualEffect;
	
protected:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon;
	
///////////////////////////////////////////////////
public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() {return BehaviorTree;}
	FORCEINLINE ACPatrolPath* GetPatrolPath(){return PatrolPath;}
	
public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;
	
protected:
	virtual void Damaged() override;
	virtual void Dead() override;
	virtual void Idle() override;
	virtual void End_DoAction() override;

protected:
	bool IsInFront();

public:
	virtual void ShowHPBar(){}
	
///////-Interface-///////	
public:
	void End_Damaged() override;
	void End_GuardImpact() override;
};
