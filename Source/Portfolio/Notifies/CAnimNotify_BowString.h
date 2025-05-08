#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_BowString.generated.h"

UCLASS()
class PORTFOLIO_API UCAnimNotify_BowString : public UAnimNotify
{
	GENERATED_BODY()
public:
	UCAnimNotify_BowString();

private:
	UPROPERTY(EditAnywhere)
	bool LastAttack = false;
	
private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
