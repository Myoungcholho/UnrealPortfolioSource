#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CItemStructures.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	General ,				// 일반 아이템
	Consumable ,			// 소모품 (포션 등)
	Coin ,                  // 화폐
	XP ,                    // 경험치 아이템
	Useable,				// 사용 가능한 아이템 (기타)
	Max
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
	Unique,
};

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	General,
	QuestItems,
	Consumables
};

UENUM(BlueprintType)
enum class EConsumableInfo : uint8
{
	Health,
	Stamina,
	Mana,
	Buff,
};

USTRUCT(BlueprintType)
struct FConsumableData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	EConsumableInfo ConsumableType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	float ConsumablePower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable")
	float ConsumableDuration;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText InteractionText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EItemType Type;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EItemCategory Category;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString IconPath;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Cost;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsStackable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 MaxStackSize;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DropRate;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FConsumableData ConsumableInfo;
};

//-----------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FDropData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FString MonsterID;

	UPROPERTY(EditAnywhere)
	FString ItemID;

	UPROPERTY(EditAnywhere)
	int32 MinDrop;

	UPROPERTY(EditAnywhere)
	int32 MaxDrop;

	UPROPERTY(EditAnywhere)
	float DropRate;
};

UCLASS()
class PORTFOLIO_API UCItemStructures : public UObject
{
	GENERATED_BODY()
	
};
