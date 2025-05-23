#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndParkour.generated.h"

UCLASS()
class PORTFOLIO_API UCAnimNotify_EndParkour : public UAnimNotify
{
	GENERATED_BODY()
private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
