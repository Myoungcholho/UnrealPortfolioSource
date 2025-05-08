#include "FToolBarButton.h"
#include "Portfolio/Global.h"

FToolBarButton::FToolBarButton() : TCommands<FToolBarButton>("ToolBar_Button",
	FText(), NAME_None, FAppStyle::GetAppStyleSetName())
{
	Command_Slomo = MakeShareable(new FUICommandList());
}

FToolBarButton::~FToolBarButton()
{
	if (Command_Slomo.IsValid())
		Command_Slomo.Reset();
}

void FToolBarButton::RegisterCommands()
{
#define LOCTEXT_NAMESPACE ""
	UI_COMMAND(btnCommandInfo1,"Slow","",EUserInterfaceActionType::Button,FInputChord());
	UI_COMMAND(btnCommandInfo2,"Fast","",EUserInterfaceActionType::Button,FInputChord());
	UI_COMMAND(btnCommandInfo3,"SlowestSpeed","",EUserInterfaceActionType::Button,FInputChord());
	UI_COMMAND(btnCommandInfo4,"DefaultSpeed","",EUserInterfaceActionType::Button,FInputChord());
#undef LOCTEXT_NAMESPACE

	FExecuteAction slow = FExecuteAction::CreateRaw(this, &FToolBarButton::SlowSlomo);
	Command_Slomo->MapAction(btnCommandInfo1,slow);

	FExecuteAction fast = FExecuteAction::CreateRaw(this, &FToolBarButton::FastSlomo);
	Command_Slomo->MapAction(btnCommandInfo2,fast);

	FExecuteAction slowest = FExecuteAction::CreateRaw(this, &FToolBarButton::SlowestSlomo);
	Command_Slomo->MapAction(btnCommandInfo3,slowest);

	FExecuteAction defaultAction = FExecuteAction::CreateRaw(this, &FToolBarButton::DefaultSlomo);
	Command_Slomo->MapAction(btnCommandInfo4,defaultAction);
}

void FToolBarButton::OnPIEStarted(bool bIsSimulating)
{
	float SlomoValue = Get().GamePlaySpeed;
		
	UWorld* World = nullptr;
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (Context.WorldType == EWorldType::PIE)
		{
			World = Context.World();
			break;
		}
	}

	if (World)
	{
		World->GetWorldSettings()->SetTimeDilation(SlomoValue);
		FLog::Log(FString::Printf(TEXT("PIE Start - Slomo Set: %.2f"), SlomoValue));
	}
}

void FToolBarButton::SlowSlomo()
{
	GamePlaySpeed = FMath::Max(GamePlaySpeed - 0.1f, 0.1f);
	
	UWorld* World = nullptr;
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game)
		{
			World = Context.World();
			break;
		}
	}

	if (World)
	{
		World->GetWorldSettings()->SetTimeDilation(GamePlaySpeed);
	}

	FLog::Log(GamePlaySpeed);
	FLog::Log("SlowSlomo!");
}

void FToolBarButton::FastSlomo()
{
	GamePlaySpeed = FMath::Min(GamePlaySpeed + 0.1f, 1.0f);
	
	UWorld* World = nullptr;
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game)
		{
			World = Context.World();
			break;
		}
	}

	if (World)
	{
		World->GetWorldSettings()->SetTimeDilation(GamePlaySpeed);
	}

	FLog::Log(GamePlaySpeed);
	FLog::Log("FastSlomo!");
}

void FToolBarButton::SlowestSlomo()
{
	GamePlaySpeed = 0.1f;
	
	UWorld* World = nullptr;
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game)
		{
			World = Context.World();
			break;
		}
	}

	if (World)
	{
		World->GetWorldSettings()->SetTimeDilation(GamePlaySpeed);
	}
	
	FLog::Log("SlowestSlomo!");
}

void FToolBarButton::DefaultSlomo()
{
	GamePlaySpeed = 1.0f;
	
	UWorld* World = nullptr;
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game)
		{
			World = Context.World();
			break;
		}
	}

	if (World)
	{
		World->GetWorldSettings()->SetTimeDilation(GamePlaySpeed);
	}
	
	FLog::Log("DefaultSlomo!");
}
