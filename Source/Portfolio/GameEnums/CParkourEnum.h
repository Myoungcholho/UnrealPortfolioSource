

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CParkourEnum.generated.h"

UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	Center = 0, Head, Foot, Left, Right, Land, Max,
};

UENUM(BlueprintType)
enum class EParkourType : uint8
{
	Climb = 0, Fall, Slide, Short, Normal, Wall, Max,
};

USTRUCT(BlueprintType)
struct FParkourData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EParkourType Type;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	FName SectionName;

	UPROPERTY(EditAnywhere)
	float MinDistance;

	UPROPERTY(EditAnywhere)
	float MaxDistance;

	UPROPERTY(EditAnywhere)
	float Extent;

public:
	void PlayMontage(class ACharacter* InCharacter);
};

UCLASS()
class PORTFOLIO_API UCParkourEnum : public UObject
{
	GENERATED_BODY()
	
};
