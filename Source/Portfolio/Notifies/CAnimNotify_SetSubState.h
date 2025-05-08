#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Portfolio/Components/CStateComponent.h"
#include "CAnimNotify_SetSubState.generated.h"

UCLASS()
class PORTFOLIO_API UCAnimNotify_SetSubState : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	ESubStateType StateType;
	
private:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;	
};
