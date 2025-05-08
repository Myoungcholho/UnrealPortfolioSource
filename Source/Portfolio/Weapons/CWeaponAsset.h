#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponStructures.h"

#include "CWeaponAsset.generated.h"

UCLASS()
class PORTFOLIO_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

// 전략 패턴(객체의 동작을 외부에서 설정하고 변경)
private:
	// 무기 Type
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType = EWeaponType::Max;

	// 무기 정보
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;
	
	//////////////////////////////////////////////
	///장착 정보
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
	FEquipmentData EquipData;
	//////////////////////////////////////////////
	// 공격 정보 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCSubAction> SubActionClass;
	
	// 공격 데이터
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FDoHitData> HitDatas;

	// 커멘드 데이터
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> CommandActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FDoHitData> CommandHitDatas;

	// 암살 데이터
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAssassination> AssassinationClass;
	
	UPROPERTY(EditAnywhere)
	TArray<FExecutionData> AssassinationDatas;
	
	// 공중 콤보 데이터
	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoAirActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FDoHitData> HitAirDatas;

public:
	FORCEINLINE EWeaponType GetWeaponType(){return WeaponType;}

public:
	UCWeaponAsset();
	
public:
	void BeginPlay(class ACharacter* InOwner,class UCWeaponData** OutData);
};
