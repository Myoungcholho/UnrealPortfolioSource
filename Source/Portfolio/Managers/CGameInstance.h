#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

UCLASS()
class PORTFOLIO_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	class UCAssassinationManager* GetAssassinationManager() const  { return AssassinationManager; }
	class UCItemManager* GetItemManager() const {return ItemManager;}
	class UCTeamCombatManager* GetTeamCombatManager() const{return TeamCombatManager;}
	
	UFUNCTION(BlueprintCallable)	
	class UCCursorManager* GetCursorManager() const {return CursorManager;}
private:
	UPROPERTY(Transient)
	class UCAssassinationManager* AssassinationManager;

	UPROPERTY(Transient)
	class UCItemManager* ItemManager;

	UPROPERTY(Transient)
	class UCCursorManager* CursorManager;

	UPROPERTY(Transient)
	class UCTeamCombatManager* TeamCombatManager;
};
