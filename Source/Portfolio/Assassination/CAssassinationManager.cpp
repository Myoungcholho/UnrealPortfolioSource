#include "../Assassination/CAssassinationManager.h"

#include "EngineUtils.h"
#include "IAssassination.h"
#include "Portfolio/Utilities/FHelpers.h"
#include "GameFramework/Character.h"

void UCAssassinationManager::TryAssassinate(ACharacter* Assassin, ACharacter* Target,FExecutionData& InData, EWeaponType InType)
{
	//  1. 인터페이스 캐스팅
	IIAssassination* AssassinInterface = Cast<IIAssassination>(Assassin);
	IIAssassination* TargetInterface = Cast<IIAssassination>(Target);

	//  2. 캐스팅 실패 시 return
	if (!AssassinInterface || !TargetInterface)
		return;

	//  3. 암살 가능 확인
	if (!TargetInterface->CanBeAssassinated(InType,InData.bIsBackExecution))
		return;

	FExecutionMontageData* PlayLastMontage = nullptr;
	PlayLastMontage = TargetInterface->GetLastAssassinMontage(InData.Damage);
	
	// 슬로우
	TArray<ACharacter*> slowTargets;
	slowTargets.Add(Assassin);
	slowTargets.Add(Target);
	
	FHelpers::ApplySlowMotion(Assassin->GetWorld(),0.1f,3.0f,slowTargets);
	
	// 암살 실행
	if(!!PlayLastMontage)
	{
		// 대상자는 재셋팅, 피격자는 알아서 할것임
		if(PlayLastMontage->AssassinMontage != nullptr)
		{
			InData.ActionMontage = PlayLastMontage->AssassinMontage;
			InData.ActionPlayRate = PlayLastMontage->AssassinPlayRate;
			InData.ExecutionOffset = PlayLastMontage->ExecutionOffset;
			InData.bIsBackExecution = PlayLastMontage->bIsBackExecution;
			InData.Type = EExecutionType::LastExecution;
		}
	}
	
	TargetInterface->OnAssassinated(Assassin,InData,InType);
	AssassinInterface->ExecuteAssassination(Target,InData,InType);
}