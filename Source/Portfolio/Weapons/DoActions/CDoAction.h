#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../CWeaponStructures.h"
#include "CDoAction.generated.h"

UCLASS()
class PORTFOLIO_API UCDoAction : public UObject
{
	GENERATED_BODY()

private:
	friend class UCAnimNotify_BeginAction;
	friend class UCAnimNotify_EndAction;
	friend class UCAnimNotify_SubAction_Katana;
	friend class UCBTTaskNode_Action;
	friend class ACPlayer;
	friend class UCWeaponComponent;
	
public:
	FORCEINLINE bool GetBeginAction(){return bBeginAction;}
	FORCEINLINE bool GetInAction(){return bInAction;}
	
public:
	UCDoAction();

	virtual void BeginPlay(class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FDoHitData>& InHitDatas,
		const TArray<FDoActionData>& InCommandActionDatas,
		const TArray<FDoHitData>& InCommandHitDatas,
		const TArray<FDoActionData>& InDoAirActionDatas,
		const TArray<FDoHitData>& InHitAirDatas);
	
	virtual void Tick(float InDeltaTime) {};
	
public:
	virtual void DoAction();
	virtual void DoAction(int32 InPlayIndex);
	void DoCommandAction(int32 InPlayIndex);

public:
	const TArray<FDoActionData>& GetCommandActionDatas() const;
	
protected:
	virtual void Begin_DoAction();
	virtual void End_DoAction();

	virtual void Begin_Air_DoAction();
	virtual void End_Air_DoAction(){}

public:
	UFUNCTION()
	virtual void OnBeginEquip() {}

	UFUNCTION()
	virtual void OnEndEquip() {}
	
	UFUNCTION()
	virtual void OnUnequip() {}

// 밑의 함수들은 Collision이 켜지고 꺼지거나
// Overlap되거나 하면 호출됨
public:
	UFUNCTION()
	virtual void OnAttachmentBeginCollision(){}
	
	UFUNCTION()
	virtual void OnAttachmentEndCollision(){}
	
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) {}
   
	UFUNCTION()
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {}
protected:
	// 몽타주 임의로 실행하기 위한 더티변수
	bool bBeginAction;
	bool bInAction;
private:
	bool bBeginAirAction;
	
protected:
	// DoAction 실행할 캐릭터
	class ACharacter* OwnerCharacter;
	// World
	class UWorld* World;

	// 움직임 제한하려고 가져옴
	class UCMoveComponent* Movement;
	// 상태 변경하려고 가져옴
	class UCStateComponent* State;

	// 공격 데이터
	TArray<FDoActionData> DoActionDatas;
	TArray<FDoHitData> HitDatas;

	// 커맨드 공격 데이터
	TArray<FDoActionData> DoCommandActionDatas;
	TArray<FDoHitData> HitCommandDatas;
	int32 CommandIndex;		// 현재 공격 인덱스
	bool bCommandAction;	// 일반/콤보 데미지 처리용

	// 공중 콤보 공격 데이터
	TArray<FDoActionData> DoAirActionDatas;
	TArray<FDoHitData> HitAirDatas;
	bool bAirAction;		// 공중 공격 데이터 처리용
};
