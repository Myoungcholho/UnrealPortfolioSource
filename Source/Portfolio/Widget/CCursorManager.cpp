#include "../Widget/CCursorManager.h"
#include "../Global.h"

UCCursorManager::UCCursorManager()
{
	count = 0;
}

void UCCursorManager::ShowCursor()
{
	// 안전한 World 가져오기
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	CheckNull(World);

	APlayerController* PlayerController = World->GetFirstPlayerController();
	CheckNull(PlayerController);

	// 마우스 커서 활성화 후 카운트 증가
	PlayerController->SetShowMouseCursor(true);

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);  // 마우스가 화면 밖으로 나갈 수 있도록 설정
	InputMode.SetHideCursorDuringCapture(false);  // 마우스 캡처 시 커서 숨김 해제
	PlayerController->SetInputMode(InputMode);
	
	++count;
}

void UCCursorManager::HideCursor()
{
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	CheckNull(World);

	APlayerController* PlayerController = World->GetFirstPlayerController();
	CheckNull(PlayerController);

	// 카운트 감소 후 체크
	--count;
	if (count <= 0)
	{
		count = 0;
		PlayerController->SetShowMouseCursor(false);
		
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}
