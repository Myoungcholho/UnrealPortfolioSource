// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CCameraBlendAsset.generated.h"

USTRUCT()
struct FCameraBlendData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName BlendName;

	UPROPERTY(EditAnywhere)
	FVector Location;

	UPROPERTY(EditAnywhere)
	FRotator Rotation;

	UPROPERTY(EditAnywhere)
	float BlendInTime = 0.5f;

	UPROPERTY(EditAnywhere)
	float ReturnTime = 3.0f;
};

UCLASS()
class PORTFOLIO_API UCCameraBlendAsset : public UDataAsset
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category="Datas")
	TArray<FCameraBlendData> BlendDatas;
public:
	const FCameraBlendData* FindBlendDataByName(FName InName) const;
		
};
