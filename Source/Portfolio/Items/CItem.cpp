#include "../Items/CItem.h"
#include "../Global.h"

#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Components/Widget.h"
#include "Portfolio/Components/CInventoryComponent.h"


ACItem::ACItem()
{
	FHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	FHelpers::CreateComponent<USphereComponent>(this, &Collision, "Collision", Root);
	FHelpers::CreateComponent<UWidgetComponent>(this, &Widget, "Widget",Collision);
    
	FHelpers::CreateComponent<UAudioComponent>(this, &Audio, "Audio", Collision);
	FHelpers::CreateComponent<UNiagaraComponent>(this, &Aura, "ItemAura", Collision);
	FHelpers::CreateComponent<UNiagaraComponent>(this, &Trail, "TrailVFX", Aura);

	Collision->SetNotifyRigidBodyCollision(true);
}


void ACItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACItem::SetItemInfo(FItemData& InData,class UNiagaraSystem* InVFX, class UNiagaraSystem* InTrail,
	class USoundBase* InSFX)
{
	Data = InData;
	FLog::Log(Data.Name);
	
	Aura->SetAsset(InVFX);
	Trail->SetAsset(InTrail);
	Audio->SetSound(InSFX);
	
	Trail->Activate();
	Audio->Play();

	Collision->SetSimulatePhysics(true);
	
	FVector RandomDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(FVector(0, 0, 1), 30.0f);
	float Speed = 500.0f;
	Collision->SetAllPhysicsLinearVelocity(RandomDirection * Speed);

	SetItemName(Data.Name);
}

void ACItem::Interact(class AActor* Interactor)
{
	// 인벤토리에 추가
	UCInventoryComponent* inventory = FHelpers::GetComponent<UCInventoryComponent>(Interactor);
	CheckNull(inventory);

	inventory->AddItem(Data);
	Destroy();
}
