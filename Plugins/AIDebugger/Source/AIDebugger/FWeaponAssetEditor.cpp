#include "FWeaponAssetEditor.h"

const FName FWeaponAssetEditor::EditorName = "WeaponAssetEditor";

// 외부에서 에디터 Open시 호출
void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	
}

// 명시 호출하는 곳 없음. 아마 에디터 끝날때 해줘야할듯
void FWeaponAssetEditor::Shutdown()
{
	
}

// OpenWindow 너무 길어져서 따로 빼둠
void FWeaponAssetEditor::Open(FString InAssetName)
{
	
}

// 에디터 닫기 요청이 들어왔을 때 실행되는 콜백 함수
bool FWeaponAssetEditor::OnRequestClose(EAssetEditorCloseReason InCloseReason)
{
	return FAssetEditorToolkit::OnRequestClose(InCloseReason);
	
}

// 모든 WeaponAsset은 고유값이 같으므로 여러 개 열리지 않음
FName FWeaponAssetEditor::GetToolkitFName() const
{
	return EditorName;	
}

// 에디터 탭에 표시될 이름 반환
FText FWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);
}

// 월드 센트릭 모드에서 탭 제목 앞에 붙는 문자열
FString FWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();
}

// 에디터 탭 등록하는 함수 (탭 매니저에 알리는 역할)
void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	
}

