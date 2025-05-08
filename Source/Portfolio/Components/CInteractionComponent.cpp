#include "../Components/CInteractionComponent.h"
#include "../Global.h"
#include "Portfolio/Items/IInteractable.h"

UCInteractionComponent::UCInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCInteractionComponent::ExecuteInteraction()
{
	CheckNull(GetOwner());

	
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	
	TArray<AActor*> FoundActors;
	FVector Center = owner->GetActorLocation();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(owner);
	
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		Radius,
		ObjectTypes,		// 검색할 타입 Actor 
		ActorClassFilter,	// 필터 Actor
		IgnoreActors,
		FoundActors
	);

	if(!bHit) return;

	for(AActor* actor : FoundActors)
	{
		IIInteractable* interactable = Cast<IIInteractable>(actor);
		if(interactable == nullptr) continue;

		interactable->Interact(owner);
		break;
	}
	//FLog::Log("Call");
}