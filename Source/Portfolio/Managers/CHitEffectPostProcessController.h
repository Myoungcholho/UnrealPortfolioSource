#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CHitEffectPostProcessController.generated.h"

UCLASS()
class PORTFOLIO_API UCHitEffectPostProcessController : public UObject
{
	GENERATED_BODY()
public:
	// 초기화 함수 (생성자 대신 사용)
	void Initialize(UWorld* WorldContext);

	// 특정 태그를 가진 Post Process Volume 찾기
	void FindPostProcessVolumes();

	// Post Process 효과 활성화
	void EnableHitEffect(float InTime);

	// Post Process 효과 비활성화
	void DisableHitEffect();

private:
	// Post Process Volume 목록
	class APostProcessVolume* PostProcessVolume;

	// 월드 컨텍스트
	UWorld* World;

	UMaterialInstanceDynamic* DynamicMaterialInstance;
	
	// 일정 시간 후 효과를 끄기 위한 타이머
	FTimerHandle TimerHandle_DisableEffect;
};
