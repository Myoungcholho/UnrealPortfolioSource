#include "../Widget/CAssassinationTargetWidget.h"
#include "../Global.h"
#include "Components/CapsuleComponent.h"
#include "Portfolio/Components/CDetectionComponent.h"

void UCAssassinationTargetWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UCAssassinationTargetWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	return;
	// 🔥 TargetActor가 없으면 UI 숨기기
	if (!TargetActor)
	{
		SetRenderOpacity(0.7f); 
		return;
	}

	// 🔥 플레이어 컨트롤러 가져오기
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		SetRenderOpacity(0.f);
		return;
	}

	// 🔥 TargetActor의 월드 위치를 가져와서 머리 높이로 조정
	FVector WorldLocation = TargetActor->GetActorLocation();

	// ✅ 타겟의 캡슐 높이를 고려하여 머리 위치로 이동
	float TargetHeight = 100.0f;  // 기본값 (타겟의 높이에 따라 조정 필요)
	if (TargetActor->IsA<ACharacter>()) 
	{
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
		if (TargetCharacter)
		{
			TargetHeight = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
		}
	}

	WorldLocation.Z += TargetHeight;  // ✅ 타겟의 머리 위로 UI 위치 이동

	// 🔥 3D → 2D 변환
	FVector2D ScreenPosition;
	bool bProjected = PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition);

	if (bProjected)
	{
		SetRenderOpacity(1.f);  // ✅ UI 표시
		SetPositionInViewport(ScreenPosition, false);  // ✅ UI 위치 업데이트
	}
	else
	{
		SetRenderOpacity(0.f);  // ✅ 타겟이 화면 밖에 있으면 숨김
	}
}

void UCAssassinationTargetWidget::OnTargetChanged(AActor* NewTarget)
{
	TargetActor = NewTarget;
}
