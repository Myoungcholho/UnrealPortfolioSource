#include "FWeaponContextMenu.h"

#include "FWeaponAssetEditor.h"
#include "Portfolio/Global.h"
#include "Portfolio/Weapons/CWeaponAsset.h"

/*// 우클릭 시 나타날 카테고리 데이터 저장, 아래 GetCategories에서 기능 실행
FWeaponContextMenu::FWeaponContextMenu(EAssetTypeCategories::Type InType)
{
	Category = InType;
}

FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::White;
}

// 에디터에 노출될 이름
FText FWeaponContextMenu::GetName() const
{
	return FText::FromString("WeaponAssetData");
}

// 에디터에 노출될 클래스 메타데이터 정보
UClass* FWeaponContextMenu::GetSupportedClass() const
{
	//return UCWeaponAsset::StaticClass();
	return nullptr;
}

// 용도가 무엇인지 모르겠음 
uint32 FWeaponContextMenu::GetCategories()
{
	return Category;
}

// 지정된 메타데이터 클래스 Open시 실행할 메서드
void FWeaponContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects,
	const EAssetTypeActivationOpenedMethod OpenedMethod, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//FAssetTypeActions_Base::OpenAssetEditor(InObjects, OpenedMethod, EditWithinLevelEditor);
	
	if (InObjects.Num() < 1)
		return;

	FWeaponAssetEditor::OpenWindow(InObjects[0]->GetName());
}*/
