#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_ControlRotation.generated.h"

UENUM(BlueprintType)
enum class EControlRotationMode : uint8
{
	Lock,	
	Unlock
};

UCLASS()
class PORTFOLIO_API UCAnimNotify_ControlRotation : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category="Option")
	EControlRotationMode type;

private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
