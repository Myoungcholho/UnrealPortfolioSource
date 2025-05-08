#pragma once

#include "CoreMinimal.h"
#include "CItemStructures.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "CItem.generated.h"

UCLASS()
class PORTFOLIO_API ACItem : public AActor , public IIInteractable
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UWidgetComponent* Widget;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class USphereComponent* Collision;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UAudioComponent* Audio;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UNiagaraComponent* Aura;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UNiagaraComponent* Trail;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetItemName(const FString& InName);
	
public:	
	ACItem();

protected:
	virtual void BeginPlay() override;
	
public:
	void SetItemInfo(FItemData& InData,class UNiagaraSystem* InVFX, class UNiagaraSystem* InTrail, class USoundBase* InSFX);

	// void OnComponentHit BP에 구현.

public:
	// Interaction Interface 구현
	virtual void Interact(class AActor* Interactor) override;

private:
	FItemData Data;
};
