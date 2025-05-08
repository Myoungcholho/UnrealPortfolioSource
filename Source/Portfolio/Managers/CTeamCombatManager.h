#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CTeamCombatManager.generated.h"

UCLASS()
class PORTFOLIO_API UCTeamCombatManager : public UObject
{
	GENERATED_BODY()
private:
	// 팀 구분
	UPROPERTY()
	TMap<AActor*, int32> ActorToTeamID;

	// 접근 토근 <본인, 접근 대상 Actor*>
	TMap<AActor*, TSet<AActor*>> TargetToApproachers;

	UPROPERTY()
	int32 MaxApproachersPerTarget = 1;  // 설정값
	
public:
	void RegisterActor(AActor* Actor, int32 TeamID);
	void UnregisterActor(AActor* Actor);
	int32 GetTeamID(AActor* Actor) const;

public:
	// 접근 시도: 성공 시 true
	bool TryRequestApproach(AActor* Requester, AActor* Target);

	// 접근 해제
	void ReleaseApproach(AActor* Requester, AActor* Target);

	// 현재 접근 중인지
	bool IsApproaching(AActor* Requester, AActor* Target) const;

	// 타겟에 접근한 AI 수
	int32 GetApproacherCount(AActor* Target) const;
	
};
