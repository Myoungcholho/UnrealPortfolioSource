// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAIDebuggerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FExtender> Extender;
	TSharedPtr<class FWeaponContextMenu> ContextMenu;
public:
	void OnAdd_ToolBarButton(FToolBarBuilder& InBuilder);
};
