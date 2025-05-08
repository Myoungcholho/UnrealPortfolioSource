#include "../Components/FStateDebugVisitor.h"

#include "CStateComponent.h"

void FStateDebugVisitor::Visit(class UCStateComponent* StateComp)
{
	Name = StateComp->GetOwner()->GetName();
	CurrentState = StateComp->GetCurrentType();
	PrevState = StateComp->GetPrevType();
	CurrentSubState = StateComp->GetCurrentSubType();
	PrevSubState = StateComp->GetPrevSubType();
	CurrentHit = StateComp->GetCurrentHitResponseType();
	PrevHit = StateComp->GetPrevHitResponseType();
}
