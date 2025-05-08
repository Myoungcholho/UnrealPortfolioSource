#pragma once

#include "CoreMinimal.h"
#include "CWeaponStructures.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"

UCLASS()
class PORTFOLIO_API UCWeaponData : public UObject
{
	GENERATED_BODY()
private:
	friend class UCWeaponAsset;

public:
	FORCEINLINE EWeaponType GetWeaponType() {return WeaponType;}
	
public:
	UCWeaponData();

public:
	FORCEINLINE class ACAttachment* GetAttachment(){return Attachment;}
	FORCEINLINE class UCEquipment* GetEquipment(){return Equipment;}
	FORCEINLINE class UCDoAction* GetDoAction(){return DoAction;}
	FORCEINLINE class UCSubAction* GetSubAction(){return SubAction;}
	FORCEINLINE class UCAssassination* GetAssassination(){return Assassination;}
	
private:
	UPROPERTY()
	EWeaponType WeaponType = EWeaponType::Max;
	
	class ACAttachment* Attachment;
	
	UPROPERTY()
	class UCEquipment* Equipment;
	
	UPROPERTY()
	class UCDoAction* DoAction;

	UPROPERTY()
	class UCSubAction* SubAction;

	UPROPERTY()
	class UCAssassination* Assassination; 
};
