#include "../Managers/CTeamCombatManager.h"
#include "../Global.h"

void UCTeamCombatManager::RegisterActor(AActor* Actor, int32 TeamID)
{
	ActorToTeamID.Add(Actor, TeamID);
	FLog::Log("UCTeamCombatManager : ");
	FLog::Log(Actor->GetName());
}

void UCTeamCombatManager::UnregisterActor(AActor* Actor)
{
	ActorToTeamID.Remove(Actor);
}

int32 UCTeamCombatManager::GetTeamID(AActor* Actor) const
{
	if (const int32* Found = ActorToTeamID.Find(Actor))
		return *Found;

	return -1; // Unknown
}

bool UCTeamCombatManager::TryRequestApproach(AActor* Requester, AActor* Target)
{
	if (!Requester || !Target) return false;
	if (Requester == Target) return false;

	// 접근 대상자들, Set을 가져옴
	TSet<AActor*>& Approachers = TargetToApproachers.FindOrAdd(Target);

	// 이미 접근 중이면 허용
	if (Approachers.Contains(Requester))
		return true;

	// 정원 초과면 접근 불가
	if (Approachers.Num() >= MaxApproachersPerTarget)
		return false;

	// 등록
	Approachers.Add(Requester);
	return true;
}

void UCTeamCombatManager::ReleaseApproach(AActor* Requester, AActor* Target)
{
	if (!Requester || !Target) return;

	if (TargetToApproachers.Contains(Target))
	{
		TargetToApproachers[Target].Remove(Requester);

		// 접근자가 모두 빠지면 테이블 정리
		if (TargetToApproachers[Target].Num() == 0)
		{
			TargetToApproachers.Remove(Target);
		}
	}
}

bool UCTeamCombatManager::IsApproaching(AActor* Requester, AActor* Target) const
{
	if (!Requester || !Target) return false;

	// Requester가 Target에 접근 중이라면 true 반환
	if (const TSet<AActor*>* Approachers = TargetToApproachers.Find(Target))
	{
		return Approachers->Contains(Requester);
	}
	
	return false;
}

int32 UCTeamCombatManager::GetApproacherCount(AActor* Target) const
{
	if (!Target) return 0;

	// 있다면 배열 수 반환(접근 중인 객체 수)
	if (const TSet<AActor*>* Approachers = TargetToApproachers.Find(Target))
		return Approachers->Num();

	return 0;
}
