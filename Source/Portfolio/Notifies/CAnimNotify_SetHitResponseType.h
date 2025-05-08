#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Portfolio/GameEnums/CStateEnum.h"

#include "CAnimNotify_SetHitResponseType.generated.h"

UCLASS()
class PORTFOLIO_API UCAnimNotify_SetHitResponseType : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category="Type")
	EHitResponseType Type;
private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	
};
