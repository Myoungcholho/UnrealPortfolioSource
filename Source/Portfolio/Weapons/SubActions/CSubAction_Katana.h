#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/SubActions/CSubAction.h"
#include "CSubAction_Katana.generated.h"

UCLASS()
class PORTFOLIO_API UCSubAction_Katana : public UCSubAction
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class UCDataAsset_Katana_Guard* DataAsset;

public:
	virtual void Begin_SubAction() override;
	virtual void End_SubAction() override;
	virtual void Begin_UnSubAction() override;
	virtual void End_UnSubAction() override;

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	virtual void Tick(float InDeltaTime) override;

public:
	virtual void Pressed() override;
	virtual void Released() override;

public:
	virtual void OnUnEquip() override;

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

public:
	// 가드 실행
	void PlayGuard(class ACharacter* InAttacker);
	void PlayGuardBroken();
	
	void End_Parry();
	void End_Broken();
	void Reset();
private:
	int GuardIndex = 0;
	int GuardStack = 0;
	bool GuardBroken = false; // True인 상태에서 가드 시 튕겨나감
	
	int GuardHitIndex =0;
};