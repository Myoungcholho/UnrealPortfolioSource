#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CItemRarityAsset.generated.h"

UCLASS()
class PORTFOLIO_API UCItemRarityAsset : public UDataAsset
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	TArray<UNiagaraSystem*> VFXEffects;

	UPROPERTY(EditAnywhere)
	TArray<UNiagaraSystem*> TrailEffects;

	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> Sounds;

public:
	UFUNCTION(BlueprintCallable, Category = "Effects")
	FORCEINLINE TArray<UNiagaraSystem*>& GetVFXEffects() { return VFXEffects; }

	UFUNCTION(BlueprintCallable, Category = "Effects")
	FORCEINLINE TArray<UNiagaraSystem*>& GetTrailEffects() { return TrailEffects; }

	UFUNCTION(BlueprintCallable, Category = "Sound")
	FORCEINLINE TArray<USoundBase*>& GetSounds() { return Sounds; }
};
