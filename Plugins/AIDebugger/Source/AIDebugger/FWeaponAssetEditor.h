#pragma once

#include "CoreMinimal.h"

// 데이터 에셋 에디터 커스텀 관리 클래스
class AIDEBUGGER_API FWeaponAssetEditor : public FAssetEditorToolkit
{
public:
	static void OpenWindow(FString InAssetName = "");
	static void Shutdown();

private:
	void Open(FString InAssetName);

protected:
	bool OnRequestClose(EAssetEditorCloseReason InCloseReason) override;

private:
	static TSharedPtr<FWeaponAssetEditor> Instance;

public:
	FName GetToolkitFName() const override;
	FText GetBaseToolkitName() const override;
	FString GetWorldCentricTabPrefix() const override;
	void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
///////////////////////////////////////////////////////////////////////////////

private:
	static const FName EditorName;
	
};
