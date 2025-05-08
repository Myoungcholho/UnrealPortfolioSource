#include "../Enemies/CBossLine.h"
#include "../Global.h"
#include "Portfolio/Widget/CBossHpWidgetController.h"
#include "Portfolio/Widget/CHUD.h"
#include "Portfolio/Widget/DefaultWidgetController.h"

void ACBossLine::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	CheckNull(Boss);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC == nullptr) return;

	if (ACHUD* HUD = Cast<ACHUD>(PC->GetHUD()))
	{
		TObjectPtr<UDefaultWidgetController> ptr = HUD->GetDefaultWidgetController();
		CheckNull(ptr);
		TObjectPtr<UCBossHpWidgetController> BossController = ptr->BossHpController;
		CheckNull(BossController);
		
		BossController->RegisterBoss(Boss);
	}
}
