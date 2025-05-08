#include "../../Weapons/Assassinations/CAssassination_Bow.h"
#include "../../Global.h"
#include "Portfolio/Assassination/CAssassinationManager.h"
#include "Portfolio/Components/CDetectionComponent.h"
#include "Portfolio/Managers/CGameInstance.h"

void UCAssassination_Bow::DoAssassination()
{
	Super::DoAssassination();

	ExecuteBackstab();
}

bool UCAssassination_Bow::ExecuteBackstab()
{
	UCDetectionComponent* detection = FHelpers::GetComponent<UCDetectionComponent>(OwnerCharacter);
	CheckNullResult(detection,false);

	AActor* target = detection->GetDetectionActor();
	CheckNullResult(target,false);
	ACharacter* targetCharacter = Cast<ACharacter>(target);
	CheckNullResult(targetCharacter,false);

	UCGameInstance* gameInstance = Cast<UCGameInstance>(OwnerCharacter->GetGameInstance());
	CheckNullResult(gameInstance,false);
	
	UCAssassinationManager* manager = gameInstance->GetAssassinationManager();
	CheckNullResult(manager,false);

	// 급습 데이터 확인
	if(ExecutionMap.Contains(EExecutionType::BackStab) == false)
		return false;
	
	manager->TryAssassinate(OwnerCharacter,targetCharacter,ExecutionMap[EExecutionType::BackStab],EWeaponType::Bow);
	
	return true;
}
