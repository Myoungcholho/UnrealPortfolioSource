#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class PORTFOLIO_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,Category="Path")
	bool bLoop;
	
	UPROPERTY(EditAnywhere,Category="Path")
	int32 Index;

	UPROPERTY(EditAnywhere,Category="Path")
	bool bReverse;
	
private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	class USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere)
	class UTextRenderComponent* Text;
	
public:	
	ACPatrolPath();
	void OnConstruction(const FTransform& Transform) override;

public:
	FVector GetMoveTo();
	void UpdateIndex();
};
