#pragma once

#include "CoreMinimal.h"

class AIDEBUGGER_API FToolBarButton : public TCommands<FToolBarButton>
{
public:
	FToolBarButton();
	~FToolBarButton();

public:
	virtual void RegisterCommands() override;

public:
	// 명령이 들어왔을 때 어떤 액션을 실행해야할지 저장하는 테이블
	TSharedPtr<FUICommandList> Command_Slomo;

public:
	// 어떤 작업을 입력으로 받을지, 버튼형식의 작업을 사용할지 등..
	TSharedPtr<FUICommandInfo> btnCommandInfo1;
	TSharedPtr<FUICommandInfo> btnCommandInfo2;
	TSharedPtr<FUICommandInfo> btnCommandInfo3;
	TSharedPtr<FUICommandInfo> btnCommandInfo4;

public:
	static void OnPIEStarted(bool bIsSimulating);
	
private:
	void SlowSlomo();
	void FastSlomo();
	void SlowestSlomo();
	void DefaultSlomo();

private:
	float GamePlaySpeed = 1.0f;
	
};
