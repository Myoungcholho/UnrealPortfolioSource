#include "../GameEnums/CParkourEnum.h"

#include "GameFramework/Character.h"

//#define LOG_UCParkourComponent

void FParkourData::PlayMontage(ACharacter* InCharacter)
{
#ifdef LOG_UCParkourComponent
	FString str;
	str.Append(StaticEnum<EParkourType>()->GetNameStringByIndex((int32)Type));
	str.Append(" : ");
	str.Append(Montage->GetPathName());
	
	FLog::Print(str);
#endif

	InCharacter->PlayAnimMontage(Montage, PlayRate, SectionName);
}
