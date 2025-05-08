#include "../Managers/CGameModeBase.h"
#include "../Global.h"

#include "CHitEffectPostProcessController.h"

ACGameModeBase::ACGameModeBase()
{
	static ConstructorHelpers::FClassFinder<AHUD> HUDBPClass(TEXT("/Game/Widget/BP_CHUD.BP_CHUD_C"));
	if (HUDBPClass.Succeeded())
	{
		HUDClass = HUDBPClass.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnBPClass(TEXT("/Game/Player/BP_CSamurai.BP_CSamurai_C"));
	if (DefaultPawnBPClass.Succeeded())
	{
		DefaultPawnClass = DefaultPawnBPClass.Class;
	}
}

void ACGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	PostProcessController = NewObject<UCHitEffectPostProcessController>();
	if (PostProcessController)
	{
		PostProcessController->Initialize(GetWorld());
	}
}
