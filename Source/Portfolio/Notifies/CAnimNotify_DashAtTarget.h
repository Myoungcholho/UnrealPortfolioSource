#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_DashAtTarget.generated.h"

UENUM()
enum class EDashType : uint8
{
	Dest,MoveTo
};

UCLASS()
class PORTFOLIO_API UCAnimNotify_DashAtTarget : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	EDashType DashType;
	
private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
