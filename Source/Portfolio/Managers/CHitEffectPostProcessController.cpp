#include "../Managers/CHitEffectPostProcessController.h"
#include "../Global.h"
#include "Materials/MaterialInstanceConstant.h"

void UCHitEffectPostProcessController::Initialize(UWorld* WorldContext)
{
	//FLog::Log("Call Init 11");
	World = WorldContext;
	FindPostProcessVolumes();

	//DisableHitEffect(); // 이렇게 호출하면 안되나?, Find에서 메테리얼 준비도 안됐는데 멀티스레드가 호출할 가능성 있음??
}

void UCHitEffectPostProcessController::FindPostProcessVolumes()
{
	CheckNull(World);

	// "HitEffect" 태그가 있는 모든 액터 찾기
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(World, FName("HitEffect"), FoundActors);

    // Post Process Volume만 저장
    for (AActor* Actor : FoundActors)
    {
        APostProcessVolume* PPVolume = Cast<APostProcessVolume>(Actor);
        if (PPVolume)
        {
            PostProcessVolume = PPVolume;
        	break;
        }
    }
	CheckNull(PostProcessVolume);
	
	UMaterialInstanceConstant* MaterialInstance = Cast<UMaterialInstanceConstant>(
	   StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/TestObject/M_RadialBlur_Inst.M_RadialBlur_Inst'"))
   );

	CheckNull(MaterialInstance);

	// 동적 머티리얼 생성 및 저장
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInstance, this);
	CheckNull(DynamicMaterialInstance);

	// Post Process Volume에 동적 머티리얼 추가
	PostProcessVolume->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.0f, DynamicMaterialInstance));
	DisableHitEffect();
	DynamicMaterialInstance->SetScalarParameterValue(FName("Density"), 0.0f);
}

void UCHitEffectPostProcessController::EnableHitEffect(float InTime)
{
	CheckNull(World);
	CheckNull(PostProcessVolume);
	
	DynamicMaterialInstance->SetScalarParameterValue(FName("Density"), 1.0f);

	World->GetTimerManager().ClearTimer(TimerHandle_DisableEffect);
	World->GetTimerManager().SetTimer
	(
		TimerHandle_DisableEffect,
		this,
		&UCHitEffectPostProcessController::DisableHitEffect,
		InTime,
		false
	);
	FLog::Log("Call Start Effect");
}

void UCHitEffectPostProcessController::DisableHitEffect()
{
	CheckNull(World);
	CheckNull(PostProcessVolume);
	
	DynamicMaterialInstance->SetScalarParameterValue(FName("Density"), 0.0f);
	FLog::Log("Call End Effect");
}
