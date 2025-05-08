#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CStaminaAsset.generated.h"

USTRUCT()
struct FHealthData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Health = 0.0f;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere)
	float RecoveryHealth = 5.0f;
};

USTRUCT()
struct FManaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Mana = 0.0f;

	UPROPERTY(EditAnywhere)
	float MaxMana = 100.0f;

	UPROPERTY(EditAnywhere)
	float RecoveryMana = 5.0f;
};

USTRUCT()
struct FStunData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float Stun = 0.0f;

	UPROPERTY(EditAnywhere)
	float MaxStun = 100.0f;

	UPROPERTY(EditAnywhere)
	float RecoveryStun = 5.0f;
};

UCLASS()
class PORTFOLIO_API UCStaminaAsset : public UDataAsset
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	FHealthData HealthData;

	UPROPERTY(EditAnywhere)
	FManaData ManaData;

	UPROPERTY(EditAnywhere)
	FStunData StunData;
public:
	const FHealthData& GetHealthData() const { return HealthData; }
	const FManaData& GetManaData() const { return ManaData; }
	const FStunData& GetStunData() const { return StunData; }
};
