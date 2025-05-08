#include "../Components/CCommandQueueComponent.h"

#include "CWeaponComponent.h"
#include "../Global.h"
#include "GameFramework/Character.h"
#include "Portfolio/Weapons/DoActions/CDoAction.h"

/*
 ↑↓
↓↑
←↑→
←↓↑

*/ 

FString FCommandData::CommandTypeToString()
{
	switch (CommandType)
	{
		case ECommandType::UpKey:    return TEXT("↑");
		case ECommandType::DownKey:  return TEXT("↓");
		case ECommandType::LeftKey:  return TEXT("←");
		case ECommandType::RightKey: return TEXT("→");
		default:                     return TEXT("");
	}
}

/////////////////////////////////////////////////////////////////////
UCCommandQueueComponent::UCCommandQueueComponent()
{
	
}

void UCCommandQueueComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* owner = Cast<ACharacter>(GetOwner());
	CheckNull(owner);
	
	Weapon = FHelpers::GetComponent<UCWeaponComponent>(owner);
	CheckNull(Weapon);
}

void UCCommandQueueComponent::LeftKeyDown()
{
	// Queue는 깊은 복사 지원하므로 이렇게 전달해도 괜찮다.
	FCommandData data;
	data.CommandType = ECommandType::LeftKey;
	data.Timestamp = GetWorld()->GetTimeSeconds();

	AddCommand(data);
}

void UCCommandQueueComponent::RightKeyDown()
{
	FCommandData data;
	data.CommandType = ECommandType::RightKey;
	data.Timestamp = GetWorld()->GetTimeSeconds();

	AddCommand(data);
}

void UCCommandQueueComponent::UpKeyDown()
{
	FCommandData data;
	data.CommandType = ECommandType::UpKey;
	data.Timestamp = GetWorld()->GetTimeSeconds();

	AddCommand(data);
}

void UCCommandQueueComponent::DownKeyDown()
{
	FCommandData data;
	data.CommandType = ECommandType::DownKey;
	data.Timestamp = GetWorld()->GetTimeSeconds();

	AddCommand(data);
}

///////////////////////////////////////////////////////////////////////////////
void UCCommandQueueComponent::AddCommand(const FCommandData& Command)
{
	// ClearCommands 한번 실행
	ClearCommands();
	
	// 들어온 명령을 큐에 Push
	CommandQueue.Enqueue(Command);

	LogQueueData();
}

// 커멘드 실행이 가능한지 비교
bool UCCommandQueueComponent::CommandCheck(TArray<FCommandData>& InCommand, FString InWeaponCommandArray)
{
	int32 weaponCommandLength = InWeaponCommandArray.Len();
	
	if (InCommand.Num() < weaponCommandLength)
		return false;

	// 유저 입력에서 무기 커맨드 길이만큼의 마지막 데이터를 확인
	for (int32 i = 0; i < weaponCommandLength; ++i)
	{
		int32 userCommandIndex = InCommand.Num() - weaponCommandLength + i;
		FString userCommandChar = InCommand[userCommandIndex].CommandTypeToString();

		
		TCHAR weaponCommandChar = InWeaponCommandArray[i];

		if (userCommandChar.Len() == 0 || userCommandChar[0] != weaponCommandChar)
		{
			return false;
		}
	}
	
	return true;
}

// Action Process
void UCCommandQueueComponent::ProcessCommands()
{
	// ClearCommands 한번 실행
	ClearCommands();
	CheckNull(Weapon);
	
	// 커맨드 입력 배열로 ..
	TArray<FCommandData> CommandArray;

	FCommandData Command;
	while (CommandQueue.Dequeue(Command))
	{
		CommandArray.Add(Command);
	}

	UCDoAction* doAction = Weapon->GetDoAction();
	CheckNull(doAction);

	// 현재 무기의 커맨드 데이터 얻어옴
	const TArray<FDoActionData>& CommandDatas = doAction->GetCommandActionDatas();

	// 커맨드 데이터
	TArray<TTuple<FString, int32>> weaponCommandArray;
	for(int32 i=0; i<CommandDatas.Num(); ++i)
	{
		weaponCommandArray.Add(TPair<FString, int32>(CommandDatas[i].Command, i));
	}

	// 커맨드 데이터를 문자열이 긴 순으로 재정렬
	weaponCommandArray.Sort([](const TTuple<FString, int32>& A, const TTuple<FString, int32>& B)
	{
		return A.Get<0>().Len() > B.Get<0>().Len(); // FString 길이 기준 내림차순
	});

	// 긴 커멘드부터 검사
	bool isCommandAction = false;
	for(int32 i=0; i<weaponCommandArray.Num(); ++i)
	{
		if(CommandCheck(CommandArray,weaponCommandArray[i].Key))
		{
			Weapon->CommandDoAction(weaponCommandArray[i].Value);

			isCommandAction = true;
			break;
		}
	}

	if(isCommandAction == false)
	{
		//FLog::Log("Action!");
		Weapon->DoAction();
	}
}

void UCCommandQueueComponent::ClearCommands()
{
	const float currentTime = GetWorld()->GetTimeSeconds();

	FCommandData data;
	while(CommandQueue.Peek(data))
	{
		const float timeDelta = currentTime - data.Timestamp;

		if(timeDelta >= MaxLifetime)
		{
			// 데이터 버리는 용
			FCommandData temp;
			CommandQueue.Dequeue(temp);
		}
		else
		{
			break; // 0.5초 지나지 않았다, 이후에 검사할 애들도 전부 지나지 않았으므로 Break;
		}
	}
}

void UCCommandQueueComponent::LogQueueData()
{
	TArray<FCommandData> TempArray;

	// Step 2: 큐의 데이터를 배열로 복사 (Dequeue)
	FCommandData Command;
	while (CommandQueue.Dequeue(Command))
	{
		TempArray.Add(Command); // 데이터를 임시 배열로 옮김
	}

	// Step 3: 누적 로그 문자열 생성
	FString AccumulatedLog;

	for (const FCommandData& Cmd : TempArray)
	{
		switch (Cmd.CommandType)
		{
		case ECommandType::LeftKey:
			AccumulatedLog += TEXT("←  ");
			break;

		case ECommandType::RightKey:
			AccumulatedLog += TEXT("→  ");
			break;

		case ECommandType::UpKey:
			AccumulatedLog += TEXT("↑  ");
			break;

		case ECommandType::DownKey:
			AccumulatedLog += TEXT("↓  ");
			break;

		default:
			break;
		}
	}

	FLog::Print(AccumulatedLog,10);

	// Step 4: 큐에 데이터를 다시 복원
	for (const FCommandData& Cmd : TempArray)
	{
		CommandQueue.Enqueue(Cmd); // 큐에 복원
	}
}
