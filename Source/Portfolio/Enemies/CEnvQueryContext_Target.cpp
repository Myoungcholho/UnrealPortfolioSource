#include "../Enemies/CEnvQueryContext_Target.h"
#include "../Global.h"

#include "CEnemy_AI.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Portfolio/Components/CAIBehaviorComponent.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	// 나의 target 정보를 반환
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai->GetController());

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, behavior->GetTarget());
}
