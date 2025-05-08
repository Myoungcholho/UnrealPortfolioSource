// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category="Stats")
	class UCStaminaAsset* StaminaAsset;

public:
	FORCEINLINE class UCHealthStatus* GetHealthStatus() {return Health;}
	FORCEINLINE class UCStunStatus* GetStunStatus(){return Stun;}
	FORCEINLINE class UCManaStatus* GetManaStatus(){return Mana;}
	
public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TArray<class UCStatusBase*> StatusList;
	
private:
	UPROPERTY()
	class UCHealthStatus* Health;

	UPROPERTY()
	class UCStunStatus* Stun;

	UPROPERTY()
	class UCManaStatus* Mana;
};
