#include "../UnitTest/CCameraMoveTest.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ACCameraMoveTest::ACCameraMoveTest()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ACCameraMoveTest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACCameraMoveTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACCameraMoveTest::ChangeCamera()
{
	if (CameraActors[0] == nullptr) return;  // 카메라 배열이 비어 있으면 리턴

	// 🔹 다음 카메라 인덱스로 변경
	Index = (Index + 1) % 3;  
	AActor* ChangeCameraActor = CameraActors[Index];

	if (!ChangeCameraActor) return;

	// 🔹 플레이어 컨트롤러 가져오기
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;
	
	PC->SetViewTargetWithBlend(ChangeCameraActor, 0.0f);
	
	AActor* CurrentCamera = PC->GetViewTarget();
	if (!CurrentCamera) return;

	FVector WorldPosition = CurrentCamera->GetActorLocation();  // 현재 카메라 위치
	FVector2D ScreenPosition;
	if (PC->ProjectWorldLocationToScreen(WorldPosition, ScreenPosition))
	{
		
	}
}
