#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/DoActions/CDoAction.h"
#include "CDoAction_Combo.generated.h"

UCLASS()
class PORTFOLIO_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()
// 적 피격 시 각도 보정
private:
	UPROPERTY(EditAnywhere, Category="Angle")
	float FinishAngle = 1.0f;

	UPROPERTY(EditAnywhere, Category="Angle")
	float RotationSpeed = 25.0f;

	UPROPERTY(EditAnywhere, Category="Angle")
	float AvailableAngle = 0.75f;

// 콤보 관련 bool 제어 함수
public:
	FORCEINLINE void EnableCombo(){bEnable = true;}
	FORCEINLINE void DisableCombo(){bEnable = false;}

// 공격 및 콤보 
public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;
	void Begin_Air_DoAction() override;

// 제어
public:
	void Tick(float InDeltaTime) override;

// CWeaponAsset 에서 바인딩
public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) override;
	void OnAttachmentEndCollision() override;

private:
	int32 Index;		// 현재 공격 인덱스
	int32 AirIndex = -1;		// 공중 콤보 공격 인덱스

private:
	bool bEnable;		// 콤보 구간 확인
	bool bExist;		// 콤보 공격 실행

private:
	TArray<class ACharacter*> Hitted;	// 중첩 피격 제어

private:
	class ACharacter* Candidate;		// 보정할 후보자
};
