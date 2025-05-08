#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Portfolio/Items/CItemStructures.h"
#include "CItemDropComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCItemDropComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	FString MonsterID;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* DataTable;

	// 에셋 직접할당해서 필요 없을듯
	/*UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACItem> ItemClass;*/
	
public:
	UCItemDropComponent();

protected:
	virtual void BeginPlay() override;

public:
	void ItemSpawn();
	
private:
	TArray<FDropData> DropData;
};
