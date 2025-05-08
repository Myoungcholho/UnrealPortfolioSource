#include "../Components/CVisualEffectComponent.h"
#include "../Global.h"

UCVisualEffectComponent::UCVisualEffectComponent()
{
	FHelpers::GetAsset<UMaterialInstance>(&SuperArmorMaterial,"/Script/Engine.MaterialInstanceConstant'/Game/Material/M_OverlayMaterial_Inst.M_OverlayMaterial_Inst'");
}

void UCVisualEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(GetOwner());
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	CheckNull(owner);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(owner);
	CheckNull(state);

	state->OnHitResponseTypeChanged.AddDynamic(this,&UCVisualEffectComponent::HitResponseChange);
	
	mesh = FHelpers::GetComponent<USkeletalMeshComponent>(owner);
}

void UCVisualEffectComponent::HitResponseChange(EHitResponseType InPrevType, EHitResponseType InType)
{
	CheckNull(mesh);
	
	switch (InType)
	{
	case EHitResponseType::SuperArmor:
		mesh->SetOverlayMaterial(SuperArmorMaterial);	
		break;
	case EHitResponseType::Normal:
	case EHitResponseType::AntiAirborne:
	case EHitResponseType::Max:
		mesh->SetOverlayMaterial(nullptr);
		break;
	default:
		break;
	}
}


