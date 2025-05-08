#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_Slow.generated.h"

UCLASS()
class PORTFOLIO_API UCAnimNotifyState_Slow : public UAnimNotifyState
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	float SlowRate = 0.5f;
private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
