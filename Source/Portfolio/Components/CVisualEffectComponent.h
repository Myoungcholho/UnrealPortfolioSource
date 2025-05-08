#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Portfolio/GameEnums/CStateEnum.h"
#include "CVisualEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCVisualEffectComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UMaterialInstance* SuperArmorMaterial;
	
public:	
	UCVisualEffectComponent();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HitResponseChange(EHitResponseType InPrevType, EHitResponseType InType);

private:
	class USkeletalMeshComponent* mesh;
};
