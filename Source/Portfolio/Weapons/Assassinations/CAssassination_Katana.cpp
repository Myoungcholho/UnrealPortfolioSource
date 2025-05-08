#include "../../Weapons/Assassinations/CAssassination_Katana.h"
#include "../../Global.h"
#include "Portfolio/Assassination/CAssassinationManager.h"
#include "Portfolio/Components/CDetectionComponent.h"
#include "Portfolio/Managers/CGameInstance.h"


void UCAssassination_Katana::BeginPlay(class ACharacter* InOwner, const TArray<FExecutionData>& InDatas)
{
	Super::BeginPlay(InOwner, InDatas);
}

void UCAssassination_Katana::DoAssassination()
{
	Super::DoAssassination();

	if(ExecuteGroggyTarget())
		return;
	
	ExecuteBackstab();
}

bool UCAssassination_Katana::ExecuteGroggyTarget()
{
	UCDetectionComponent* detection = FHelpers::GetComponent<UCDetectionComponent>(OwnerCharacter);
	CheckNullResult(detection,false);

	// 대상이 내 앞에 있는지 확인
	AActor* target = detection->DetectFrontTarget(150.0f);
	CheckNullResult(target,false);

	ACharacter* targetCharacter = Cast<ACharacter>(target);
	CheckNullResult(targetCharacter,false);

	UCGameInstance* gameInstance = Cast<UCGameInstance>(OwnerCharacter->GetGameInstance());
	CheckNullResult(gameInstance,false);

	UCAssassinationManager* manager = gameInstance->GetAssassinationManager();
	CheckNullResult(manager,false);

	if(ExecutionMap.Contains(EExecutionType::Groggy) == false)
		return false;

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(targetCharacter);
	CheckNullResult(state,false);

	if(state->HasStatusEffect(EStatusEffect::Weakened) == false)
		return false;

	
	manager->TryAssassinate(OwnerCharacter,targetCharacter,ExecutionMap[EExecutionType::Groggy],EWeaponType::Katana);
	
	return true;
}

bool UCAssassination_Katana::ExecuteBackstab()
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
	
	manager->TryAssassinate(OwnerCharacter,targetCharacter,ExecutionMap[EExecutionType::BackStab],EWeaponType::Katana);
	return true;
}
