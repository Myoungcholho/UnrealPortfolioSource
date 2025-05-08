#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_CameraBlend.generated.h"



UCLASS()
class PORTFOLIO_API UCAnimNotify_CameraBlend : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category="Data")
	FName BlendLocationName;

	UPROPERTY(EditAnywhere, Category="Data")
	bool bZoom = false;
private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
