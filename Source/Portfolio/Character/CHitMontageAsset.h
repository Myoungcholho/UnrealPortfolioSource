#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Weapons/CWeaponStructures.h"
#include "CHitMontageAsset.generated.h"

struct my_struct
{
	
};

UCLASS()
class PORTFOLIO_API UCHitMontageAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	// 기본 공격 피격
	UPROPERTY(EditAnywhere,Category="Montages")
	TArray<FMontageData> HitMontages;

	UPROPERTY(EditAnywhere,Category="Montages")
	TArray<FMontageData> HitCommandMontages;

	UPROPERTY(EditAnywhere,Category="Montages")
	TArray<FMontageData> HitMontages_Bow;
	
	//////////////////////////////////////////////
	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData Lying_Dead_Montage;
	
	UPROPERTY(EditAnywhere,Category="Montages")
	FHitGuardMontage ReceiveGuardData;

	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData KnockDownMontage;

	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData HitLandingMontage;

	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData AirHitMontage;

	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData AirDeadMontage;

//Dead Montage
	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData BasicDeadMontage;

	// Katana Assassination + 원콤 안난 경우
	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData BacktabMontage;
	
	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData GroggyAssainationMontage;
	
	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData BacktabMontage_Hit;

	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData GroggyAssainationMontage_Hit;

	// Bow Assassination
	UPROPERTY(EditAnywhere,Category="Bow Assassination")
	FMontageData BowBackstabMontage;

	UPROPERTY(EditAnywhere,Category="Bow Assassination")
	FMontageData BowBackstabMontage_Hit;
	
//4Way Montage
	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData ForwardMontage;

	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData BackwardMontage;

	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData LeftMontage;
	
	UPROPERTY(EditAnywhere,Category="Montages")
	FMontageData RightMontage;
};
