#include "../../Weapons/Assassinations/CAssassination.h"
#include "../../Global.h"

#include "Portfolio/Utilities/FHelpers.h"

UCAssassination::UCAssassination()
{
}

void UCAssassination::BeginPlay(class ACharacter* InOwner, const TArray<FExecutionData>& InDatas)
{
	OwnerCharacter = InOwner;
	for (const FExecutionData& Data : InDatas)
		ExecutionMap.Add(Data.Type, Data);

	Movement = FHelpers::GetComponent<UCMoveComponent>(OwnerCharacter);
	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
}

void UCAssassination::DoAssassination()
{
	//State->SetAssassinationMode();
}
