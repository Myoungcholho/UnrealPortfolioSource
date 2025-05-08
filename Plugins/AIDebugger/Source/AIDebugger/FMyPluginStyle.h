#pragma once

#include "CoreMinimal.h"

class AIDEBUGGER_API FMyPluginStyle
{
public:
	static void Regist();
	static TSharedPtr<FMyPluginStyle> Get();
	static void Unregist();

private:
	static TSharedPtr<FMyPluginStyle> Instance;
	
public:
	FMyPluginStyle();
	~FMyPluginStyle();

private:
	void RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon);
	
private:
	static const FName StyleSetName;
	
private:
	TSharedPtr<class FSlateStyleSet> StyleSet;

public:
	FSlateIcon Slomo_Up;
	FSlateIcon Slomo_Down;
	FSlateIcon Slomo_0_1;
	FSlateIcon Slomo_1_0;
};
