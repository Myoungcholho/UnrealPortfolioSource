// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IWeaponType.h"
#include "../Weapons/CAttachment.h"
#include "CAttachment_Hammer.generated.h"

UCLASS()
class PORTFOLIO_API ACAttachment_Hammer : public ACAttachment,public IIWeaponType
{
	GENERATED_BODY()

public:
	ACAttachment_Hammer();
	
protected:
	virtual void AttachTo(FName InSocketName) override;

protected:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshClub;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName HolsterClub_SocketName = "Holster_Club";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	FName HandClub_SocketName = "Hand_Club";

public:
	virtual EWeaponType RetruenWeaponType() override;
};
