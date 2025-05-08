#include "../Components/CItemDropComponent.h"
#include "../Global.h"
#include "Portfolio/Items/CItem.h"
#include "Portfolio/Items/CItemManager.h"
#include "Portfolio/Managers/CGameInstance.h"

UCItemDropComponent::UCItemDropComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MonsterID = "Melee01";
	FHelpers::GetAsset<UDataTable>(&DataTable, "/Script/Engine.DataTable'/Game/Item/DT_DropTable.DT_DropTable'");
}

void UCItemDropComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CheckNull(DataTable);
	
	TArray<FDropData*> Rows;
	DataTable->GetAllRows<FDropData>(TEXT(""), Rows);

	// 매번하면 좀 어지러울꺼같은데?
	for (FDropData* Data : Rows)
	{
		if (Data && Data->MonsterID == MonsterID)
		{
			DropData.Add(*Data);
		}
	}
}

// 플레이어 사망 시 이벤트로 호출됨
void UCItemDropComponent::ItemSpawn()
{
	CheckNull(GetOwner());
	
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetOwner()->GetGameInstance());
	CheckNull(gameInstance);

	UCItemManager* manager = gameInstance->GetItemManager();
	CheckNull(manager);
	
	FVector location = GetOwner()->GetActorLocation();
	FRotator rotator = GetOwner()->GetActorRotation();
	
	// 아이템 확률 돌리기
	for(FDropData data : DropData)
	{
		float value = FMath::RandRange(0.0f,1.0f);
		int count = FMath::RandRange(data.MinDrop,data.MaxDrop);
		
		if(value <= data.DropRate)
		{
			manager->ItemSpawn(data.ItemID,count,location,rotator);
		}
	}
}