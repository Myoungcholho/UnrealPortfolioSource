#include "../Enemies/CEnvQueryContext_Player.h"
#include "../Global.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "GameFramework/Character.h"


void UCEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	UWorld* world = QueryInstance.Owner.Get()->GetWorld();
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(world ,0);
	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData,player);
}
