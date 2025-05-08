#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Weapons/CWeaponStructures.h"
#include "CCommandQueueComponent.generated.h"

UENUM()
enum class ECommandType : uint8
{
	LeftKey = 1,
	RightKey = 2,
	UpKey = 3,
	DownKey =4,
	Max
};

USTRUCT()
struct FCommandData
{
	GENERATED_BODY()
public:
	ECommandType CommandType;
	float Timestamp;

public:
	FString CommandTypeToString();
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCCommandQueueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCCommandQueueComponent();

protected:
	virtual void BeginPlay() override;
	
private:
	const float MaxLifetime = 0.5f; 

public:
	void LeftKeyDown();
	void RightKeyDown();
	void UpKeyDown();
	void DownKeyDown();
	void ProcessCommands();
	
private:
	void AddCommand(const FCommandData& Command);
	bool CommandCheck(TArray<FCommandData>& InCommand, FString InWeaponCommandArray);
	void ClearCommands();
	void LogQueueData();
	
private:
	UPROPERTY()
	class UCWeaponComponent* Weapon;

private:
	
	
private:
	TQueue<FCommandData> CommandQueue;
};
