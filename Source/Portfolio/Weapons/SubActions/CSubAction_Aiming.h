#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/SubActions/CSubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Aiming.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimingToggle, bool, bAimingCheck);

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag;

	UPROPERTY(EditAnywhere)
	FVector CameraLocation;
};

UCLASS(Blueprintable)
class PORTFOLIO_API UCSubAction_Aiming : public UCSubAction
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
	class UCurveVector* Curve;

	UPROPERTY(EditAnywhere, Category = "Aiming")
	float AimingSpeed = 200;

	UPROPERTY(EditAnywhere, Category = "Aiming")
	FAimData AimData;
public:
	UCSubAction_Aiming();

	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	virtual void Tick(float InDeltaTime) override;

public:
	void Pressed() override;
	void Released() override;

public:
	void OnUnEquip() override;

public:
	FOnAimingToggle OnAimingToggle;
	
private:
	UFUNCTION()
	void OnAiming(FVector Output);

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private:
	FAimData OriginData;

private:
	FTimeline Timeline;

private:
	float* Bend;
	
};
