#include "FMyPluginStyle.h"

#include "Styling/SlateStyleRegistry.h"

const FName FMyPluginStyle::StyleSetName = "MyStyle";
TSharedPtr<FMyPluginStyle> FMyPluginStyle::Instance = nullptr;

void FMyPluginStyle::Regist()
{
	Instance = MakeShareable(new FMyPluginStyle);
}

TSharedPtr<FMyPluginStyle> FMyPluginStyle::Get()
{
	ensure(Instance != nullptr);
	
	return Instance;
}

void FMyPluginStyle::Unregist()
{
	if(Instance.IsValid())
		Instance.Reset();
}

///////////////////////////////////////////////////////////////////////////////

FMyPluginStyle::FMyPluginStyle()
{
	StyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	FString path = FPaths::ProjectPluginsDir() / "AIDebugger" / "Resources";
	StyleSet->SetContentRoot(path);

	RegisterIcon("Slomo_Up", path / "Icon_Slomo_Up.png",FVector2d(40,40),Slomo_Up);
	RegisterIcon("Slomo_Down", path / "Icon_Slomo_Down.png",FVector2d(40,40),Slomo_Down);
	RegisterIcon("Slomo_0_1", path / "Icon_Slomo_0_1.png",FVector2d(40,40),Slomo_0_1);
	RegisterIcon("Slomo_1_0", path / "Icon_Slomo_1_0.png",FVector2d(40,40),Slomo_1_0);
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}
	
FMyPluginStyle::~FMyPluginStyle()
{
	if(StyleSet.IsValid() == false)
		return;

	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSetName);
	StyleSet.Reset();
}

void FMyPluginStyle::RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize,
	FSlateIcon& OutSlateIcon)
{
	FSlateImageBrush* brush = new FSlateImageBrush(InPath,InIconSize);

	FString name = StyleSetName.ToString() + "." + InName;
	StyleSet->Set(FName(name),brush);
	
	OutSlateIcon = FSlateIcon(StyleSetName,FName(name));
}
