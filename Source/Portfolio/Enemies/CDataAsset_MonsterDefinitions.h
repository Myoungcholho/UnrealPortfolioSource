#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CDataAsset_MonsterDefinitions.generated.h"

UENUM()
enum class EMonsterType : uint8
{
	Normal = 0,
	Boss = 1,
	Max
};

UCLASS()
class PORTFOLIO_API UCDataAsset_MonsterDefinitions : public UDataAsset
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	EMonsterType Type;

public:
	const EMonsterType& GetMonsterType() const {return Type;}
};
