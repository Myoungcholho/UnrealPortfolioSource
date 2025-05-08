#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Weapons/CWeaponStructures.h"
#include "CWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,Category="DataAsset")
	TArray<class UCWeaponAsset*> DataAssets;
	
public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE EWeaponType GetCurrentType(){return CurrentType;}
	FORCEINLINE EWeaponType GetPrevType(){return PrevType;}
	
	FORCEINLINE bool IsUnarmedMode(){return CurrentType == EWeaponType::Max;}

public:
	class UCWeaponData* GetWeaponData(EWeaponType InType);
	
public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCEquipment* GetPrevEquipment();
	class UCDoAction* GetDoAction();
	class UCSubAction* GetSubAction();
	class UCSubAction* GetPrevSubAction(); // 바인딩 해제하기 위해 이전 무기를 가져옴
	class UCAssassination* GetAssassination();

public:
	void DoAction();
	void DoAction(int32 InPlayIndex);
	void CommandDoAction(int32 InIndex);
	
public:
	void SetUnarmedMode();
	void SetKatanaMode();
	void SetBowMode();
	void SetClubMode();

private:
	bool IsIdleMode();
	
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

//SubAction
public:
	void SubAction_Pressed();
	void SubAction_Released();

//Assassation
public:
	void DoAssassation();
	
public:
	UPROPERTY(BlueprintAssignable)
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	UPROPERTY()
	class UCStateComponent* State;

	UPROPERTY()
	class UCMoveComponent* Movement;
	
private:
	EWeaponType CurrentType = EWeaponType::Max;
	EWeaponType PrevType =  EWeaponType::Max;

private:
	UPROPERTY()
	TArray<class UCWeaponData*> Datas;
	
};
