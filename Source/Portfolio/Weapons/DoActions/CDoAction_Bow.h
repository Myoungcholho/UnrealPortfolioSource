#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/DoActions/CDoAction.h"
#include "CDoAction_Bow.generated.h"

UCLASS(Blueprintable)
class PORTFOLIO_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()
public:
	UCDoAction_Bow();
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACArrow> ArrowClass;

	UPROPERTY(EditAnywhere)
	FName StringBoneName = "bow_string_mid";
	
	virtual void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FDoHitData>& InHitDatas,
		const TArray<FDoActionData>& InCommandActionDatas,
		const TArray<FDoHitData>& InCommandHitDatas,
		const TArray<FDoActionData>& InDoAirActionDatas,
		const TArray<FDoHitData>& InHitAirDatas
	) override;
	
	void Tick(float InDeltaTime) override;

	// 콤보 관련 bool 제어 함수
public:
	FORCEINLINE void EnableCombo(){bEnable = true;}
	FORCEINLINE void DisableCombo(){bEnable = false;}
	
public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	// 화살 발사
	void ShootArrow();
	
public:
	void OnEndEquip() override;
	void OnUnequip() override;

private:
	void CreateArrow();
	void DestroyArrow();
	class ACArrow* GetAttachedArrow();
	
public:
	void CanCreateArrow();
	void SetAttachedString();
	void LastAttachedString();
	void ResetAttachedString();

	void EnableSubAction();
	void DisableSubAction();
	
private:
	UFUNCTION()
	void OnArrowEndPlay(class ACArrow* InDestroyer);

	//UFUNCTION()
	//void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

private:
	class USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;

private:
	int32 Index;		// 현재 공격 인덱스
	bool bEnable;		// 콤보 구간 확인
	bool bExist;		// 콤보 공격 실행
	bool bSubAction;	// 우클릭 눌렀는지
	
private:
	const bool* bEquipped;

private:
	TArray<class ACArrow*> Arrows;

private:
	float* Bend;
	bool bAttachedString = true;
	FVector StringOriginLocation;

private:
	UPROPERTY()
	class ACAttachment* Attachment; // Arrow에 넘겨주기 위해 저장
};
