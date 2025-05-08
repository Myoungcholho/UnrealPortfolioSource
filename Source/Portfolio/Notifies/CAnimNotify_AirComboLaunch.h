#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_AirComboLaunch.generated.h"

UCLASS()
class PORTFOLIO_API UCAnimNotify_AirComboLaunch : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	float Launch = 1000.0f;
	
private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
