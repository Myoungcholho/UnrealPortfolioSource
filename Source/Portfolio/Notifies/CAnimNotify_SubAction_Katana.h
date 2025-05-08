#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_SubAction_Katana.generated.h"

UENUM()
enum class EKatanaSubActionType : uint8
{
	None = 0, Begin_DrawGuard =1, End_DrawGuard =2,
	Begin_SheatheGuard =3, End_SheatheGuard =4, Max
};

UCLASS()
class PORTFOLIO_API UCAnimNotify_SubAction_Katana : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EKatanaSubActionType Type = EKatanaSubActionType::None;

public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
