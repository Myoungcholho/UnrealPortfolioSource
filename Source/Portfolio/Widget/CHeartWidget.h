// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CHeartWidget.generated.h"

UCLASS()
class PORTFOLIO_API UCHeartWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//OnHealthPointChanged
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Widget")
	void ToggleWidgetVisibility(uint8 InValue);
	
};
