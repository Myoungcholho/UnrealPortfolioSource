#pragma once

#include "CoreMinimal.h"
#include "../Enemies/CEnemy_AI.h"
#include "Portfolio/Assassination/IAssassination.h"
#include "CEnemy_AI_Melee.generated.h"

UCLASS()
class PORTFOLIO_API ACEnemy_AI_Melee : public ACEnemy_AI
	,public IIAssassination,public IIWeaponUser
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category="Widget")
	FVector WidgetLocation = FVector(0,0,220);

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* Widget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCEnemyWidget> WidgetClass;

public:
	ACEnemy_AI_Melee();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void HitLaunch(FDoHitData* InHitData) override;
	
	//암살
public:
	virtual bool CanBeAssassinated(EWeaponType InType,bool IsBack) override;
	virtual void OnAssassinated(ACharacter* Assassin,FExecutionData& InData,EWeaponType InType) override;// 피암살자

public:
	virtual UCWeaponComponent* GetWeaponComponent() override;

public:
	virtual void ShowHPBar() override;
	
private:
	void HideHPBar();
	void ResetHideTimer();

private:
	float HPBarTimer;
};
