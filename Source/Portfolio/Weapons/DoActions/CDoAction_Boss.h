#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/DoActions/CDoAction.h"
#include "CDoAction_Boss.generated.h"

UCLASS(Blueprintable)
class PORTFOLIO_API UCDoAction_Boss : public UCDoAction
{
	GENERATED_BODY()
public:
	virtual void DoAction(int32 InPlayIndex) override;
	void End_DoAction() override;

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) override;
	void OnAttachmentEndCollision() override;

private:
	int32 Index;		// 현재 공격 인덱스
	TArray<class ACharacter*> Hitted;
};
