#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FVector LeftDistance; //X

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FVector RightDistance; //X

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FVector PelvisDistance; //Z

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FRotator LeftRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FRotator RightRotation;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float OffsetDistance = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float InterpSpeed = 10.0f;

	/*UPROPERTY(EditAnywhere, Category= "Trace")
	TArray<FVector> Offsets;*/
	
private:
	UPROPERTY(EditAnywhere, Category = "Bone")
	FName LeftBoneName = "foot_l";

	UPROPERTY(EditAnywhere, Category = "Bone")
	FName RightBoneName = "foot_r";

public:
	FORCEINLINE const FFeetData& GetData() { return Data; }

public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		

private:
	void TraceBone(FName InName, float& OutDistance, FRotator& OutRotation);
	void Trace(FVector InStart, FVector InEnd, float& OutDistance, FRotator& OutRotation);

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	FFeetData Data;
};
