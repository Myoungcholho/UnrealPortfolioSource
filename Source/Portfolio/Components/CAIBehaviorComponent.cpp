#include "../Components/CAIBehaviorComponent.h"
#include "../Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UCAIBehaviorComponent::UCAIBehaviorComponent()
{
		
}

EAIStateType UCAIBehaviorComponent::GetType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}

bool UCAIBehaviorComponent::IsWaitMode()
{
	return GetType() == EAIStateType::Wait;
}

bool UCAIBehaviorComponent::IsEquipMode()
{
	return GetType() == EAIStateType::Equip;
}

bool UCAIBehaviorComponent::IsApproachMode()
{
	return GetType() == EAIStateType::Approach;
}

bool UCAIBehaviorComponent::IsActionMode()
{
	return GetType() == EAIStateType::Action;
}

bool UCAIBehaviorComponent::IsPatrolMode()
{
	return GetType() == EAIStateType::Patrol;
}

bool UCAIBehaviorComponent::IsDamagedMode()
{
	return GetType() == EAIStateType::Damaged;
}

bool UCAIBehaviorComponent::IsAvoidMode()
{
	return GetType() == EAIStateType::Avoid;
}

bool UCAIBehaviorComponent::IsStrafeMode()
{
	return GetType() == EAIStateType::Strafe;
}

bool UCAIBehaviorComponent::IsGuardMode()
{
	return GetType() == EAIStateType::Guard;
}

void UCAIBehaviorComponent::SetWaitMode()
{
	ChangeType(EAIStateType::Wait);
}

void UCAIBehaviorComponent::SetEquipMode()
{
	ChangeType(EAIStateType::Equip);
}

void UCAIBehaviorComponent::SetApproachMode()
{
	ChangeType(EAIStateType::Approach);
}

void UCAIBehaviorComponent::SetActionMode()
{
	ChangeType(EAIStateType::Action);
}

void UCAIBehaviorComponent::SetPatrolMode()
{
	ChangeType(EAIStateType::Patrol);
}

void UCAIBehaviorComponent::SetDamagedMode()
{
	ChangeType(EAIStateType::Damaged);
}

void UCAIBehaviorComponent::SetAvoidMode()
{
	ChangeType(EAIStateType::Avoid);
}

void UCAIBehaviorComponent::SetStrafeMode()
{
	ChangeType(EAIStateType::Strafe);
}

void UCAIBehaviorComponent::SetGuardMode()
{
	ChangeType(EAIStateType::Guard);
}

///////////////////////////////////////
FVector UCAIBehaviorComponent::GetPatrolLocation()
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}

void UCAIBehaviorComponent::SetPatrolLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(PatrolLocationKey,InLocation);
}


///////////////////////////////////////
FVector UCAIBehaviorComponent::GetLocation()
{
	return Blackboard->GetValueAsVector(LocationKey);
}

float UCAIBehaviorComponent::GetActionTime()
{
	return Blackboard->GetValueAsFloat(ActionTimeKey);
}

void UCAIBehaviorComponent::SetActionTime()
{
	CheckNull(GetWorld());
	Blackboard->SetValueAsFloat(ActionTimeKey,GetWorld()->GetTimeSeconds());
}

///////////////////////////////////////
class ACharacter* UCAIBehaviorComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}

void UCAIBehaviorComponent::ChangeType(EAIStateType InType)
{
	EAIStateType prevType = GetType();

	Blackboard->SetValueAsEnum(AIStateTypeKey,(uint8)InType);

	if(OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType,InType);
}
