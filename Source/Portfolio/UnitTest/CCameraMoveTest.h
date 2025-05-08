#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCameraMoveTest.generated.h"

UCLASS()
class PORTFOLIO_API ACCameraMoveTest : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	AActor* CameraActors[3];
	
public:	
	ACCameraMoveTest();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void ChangeCamera();

private:
	int32 Index = 0;
};
