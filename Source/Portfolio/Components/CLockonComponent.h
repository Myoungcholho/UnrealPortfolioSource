#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLockonComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCLockonComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetLockOn() const {return bLockOn;}

protected:
	UPROPERTY(EditDefaultsOnly,Category="Target")
	float Radius = 1000.0f;

	UPROPERTY(EditDefaultsOnly,Category="Target")
	float LimitAngle = 30.0f;
	
	UPROPERTY(EditDefaultsOnly,Category="Target")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditDefaultsOnly,Category="Target")
	TSubclassOf<class AActor> ActorClassFilter;
	
public:
	UCLockonComponent();
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	class UCMoveComponent* Movement;

	// 얘는 월드에서 관리될 애라 직렬화 안해도됨
	UPROPERTY()
	class ACharacter* OwnerCharacter;

public:
	// 범위내의 적을 탐색
	class AActor* GetTargetInFront();

public:
	void ChangeLockMode();
	void OnLock();
	void OffLock();
	
private:
	bool bLockOn = false;
};
