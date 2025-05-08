#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDetectionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDetectionChanged, AActor*, DetectedActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCDetectionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	FORCEINLINE AActor* GetDetectionActor(){return CurrentDetectionActor;}
	
private:
	UPROPERTY(EditDefaultsOnly, Category="DetectAssassination")
	float AssassinationRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category="DetectAssassination")
	float AssassinationDotThreshold = 0.7f;

	UPROPERTY(EditDefaultsOnly, Category="DetectAssassination")
	TSubclassOf<class UWidgetComponent> TargetWidgetClass;
public:	
	UCDetectionComponent();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable, Category="Detection")
	FOnDetectionChanged OnDetectionChanged;

public:
	// 앞) 그로기 처형 가능 Actor 얻어옴
	AActor* DetectFrontTarget(float DetectionRange);
	
private:
	// 뒤) 암살 가능 Actor 얻어옴
	AActor* DetectAssassinationTarget(AActor* InActor);
	
private:
	void ChangeTarget(AActor* InActor);

	AActor* CurrentDetectionActor;

private:
	class UWidgetComponent* TargetWidget;
};
