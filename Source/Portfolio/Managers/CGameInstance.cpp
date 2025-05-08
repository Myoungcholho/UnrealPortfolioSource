#include "../Managers/CGameInstance.h"
#include "../Global.h"

#include "CHitEffectPostProcessController.h"
#include "CTeamCombatManager.h"
#include "Portfolio/Assassination/CAssassinationManager.h"
#include "Portfolio/Items/CItemManager.h"
#include "Portfolio/Widget/CCursorManager.h"


void UCGameInstance::Init()
{
	Super::Init();

	AssassinationManager = NewObject<UCAssassinationManager>(this);
	ItemManager = NewObject<UCItemManager>(this);
	CursorManager = NewObject<UCCursorManager>(this);
	TeamCombatManager = NewObject<UCTeamCombatManager>(this);
}
