// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IWeaponType.h"
#include "../Weapons/CAttachment.h"
#include "CAttachment_Katana.generated.h"

UCLASS()
class PORTFOLIO_API ACAttachment_Katana : public ACAttachment
	,public IIWeaponType
{
	GENERATED_BODY()

public:
	ACAttachment_Katana();
	
protected:
	virtual void BeginPlay() override;
	
protected:
	virtual void AttachTo(FName InSocketName) override;

protected:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshKatana;
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName HolsterScabbard_SocketName = "Holster_Scabbard";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName HolsterKanata_SocketName = "Holster_Katana";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName HandKatana_SocketName = "Hand_Katana";

public:
	virtual EWeaponType RetruenWeaponType() override;
	
private:
	void ScabbardSpawn();
};
