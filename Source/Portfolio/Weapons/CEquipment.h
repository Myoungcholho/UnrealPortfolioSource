#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.h"
#include "Portfolio/BehaviorTree/CBTTaskNode_Equip.h"

#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnEquip);

UCLASS()
class PORTFOLIO_API UCEquipment : public UObject
{
	GENERATED_BODY()
	
private:
	friend class UCAnimNotifyState_Equip;
	friend class UCAnimNotifyState_UnEquip;
	friend class UCWeaponComponent;
	friend class UCBTTaskNode_Equip;
	
public:
	FORCEINLINE const bool* GetEquipped(){return &bEquipped;}
	FORCEINLINE bool GetBeginEquip(){return bBeginEquip;}
	
public:
	void BeginPlay(class ACharacter* InCharacter,const FEquipmentData& InData);

public:
	UFUNCTION(BlueprintNativeEvent)
	void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void UnEquip(bool InQuickUnEquip);
	virtual void UnEquip_Implementation(bool InQuickUnEquip);
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Begin_UnEquip();
	void Begin_UnEquip_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void End_UnEquip();
	void End_UnEquip_Implementation();
	
	
public:
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnEquip OnEquipmentUnEquip;

private:
	class ACharacter* OwnerCharacter;

private:
	FEquipmentData Data;

private:
	class UCStateComponent* State;
	class UCMoveComponent* Movement;

private:
	bool bEquipped;
	bool bBeginEquip;
};