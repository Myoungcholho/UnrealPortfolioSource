#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndParry.generated.h"

UENUM()
enum class EGuardType : uint8
{
	Parry =0, Broken =1, Max
};

UCLASS()
class PORTFOLIO_API UCAnimNotify_EndParry : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EGuardType GuardType;
	
public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
