#include "AIDebuggerModule.h"

#include "FMyPluginStyle.h"
#include "FToolBarButton.h"
#include "FWeaponContextMenu.h"
#include "GameplayDebugger.h"
#include "LevelEditor.h"
#include "Debugger/AIStateDebugger.h"

#define LOCTEXT_NAMESPACE "FAIDebuggerModule"

void FAIDebuggerModule::StartupModule()
{
	//Style Memroy에 저장
	FMyPluginStyle::Regist();
	Extender = MakeShareable(new FExtender());
	
	// Debugger
	{
		IGameplayDebugger::FOnGetCategory category;
		category.BindStatic(&AIStateDebugger::MakeInstance);

		IGameplayDebugger::Get().RegisterCategory("AI State",category,EGameplayDebuggerCategoryState::EnabledInGameAndSimulate,6);
		IGameplayDebugger::Get().NotifyCategoriesChanged();
	}

	// ToolBar
	{
		FToolBarButton::Register();

		// 실제로 버튼을 추가하는 함수를 바인딩한 델리게이트
		FToolBarExtensionDelegate toolBarDelegate;
		toolBarDelegate.BindRaw(this,&FAIDebuggerModule::OnAdd_ToolBarButton);

		// 위치와 실행할 명령
		Extender->AddToolBarExtension
		(
			"Play",	// 기준 위치
			EExtensionHook::After, // 기준 위치의 앞/뒤
			FToolBarButton::Get().Command_Slomo, // 명령 리스트
			toolBarDelegate // 실제로 버튼을 추가하는 함수 바인딩된 델리게이트
		);

		// 에디터에 추가
		FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		levelEditor.GetToolBarExtensibilityManager()->AddExtender(Extender);
	}

	// Bind , UnrealEd 모듈 필요
	FEditorDelegates::PostPIEStarted.AddStatic(&FToolBarButton::OnPIEStarted);

	///////////////////////////////////////////////////////////////////////////
	// 여기부터 데이터 에셋 커스텀
	/*IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type categoryType = assetTools.RegisterAdvancedAssetCategory("WeaponAsset", FText::FromString("Weapon"));

	ContextMenu = MakeShareable(new FWeaponContextMenu(categoryType));
	assetTools.RegisterAssetTypeActions(ContextMenu.ToSharedRef());*/
}

void FAIDebuggerModule::ShutdownModule()
{
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger::Get().UnregisterCategory("AI State");
	}

	FToolBarButton::Unregister();
	FMyPluginStyle::Unregist();

	/*if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		assetTools.UnregisterAssetTypeActions(ContextMenu.ToSharedRef());
	}	*/

	if (ContextMenu.IsValid())
		ContextMenu.Reset();
}

void FAIDebuggerModule::OnAdd_ToolBarButton(FToolBarBuilder& InBuilder)
{
	InBuilder.AddSeparator();	// 구분선
	InBuilder.AddToolBarButton
	(
		FToolBarButton::Get().btnCommandInfo1,
		NAME_None,FText(),
		FText::FromString("GamePlay Slow 0.1"),
		FMyPluginStyle::Get()->Slomo_Down
	);
	InBuilder.AddToolBarButton
	(
		FToolBarButton::Get().btnCommandInfo2,
		NAME_None,FText(),
		FText::FromString("GamePlay Fast 0.1"),
		FMyPluginStyle::Get()->Slomo_Up
	);
	InBuilder.AddToolBarButton
	(
		FToolBarButton::Get().btnCommandInfo3,
		NAME_None,FText(),
		FText::FromString("GamePlay Slomo 0.1"),
		FMyPluginStyle::Get()->Slomo_0_1
	);
	InBuilder.AddToolBarButton
	(
		FToolBarButton::Get().btnCommandInfo4,
		NAME_None,FText(),
		FText::FromString("GamePlay Slomo 1.0"),
		FMyPluginStyle::Get()->Slomo_1_0
	);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAIDebuggerModule, AIDebugger)