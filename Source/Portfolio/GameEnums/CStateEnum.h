#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CStateEnum.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle =0, Evade=1,Equip=2,Damaged=3,
	Action=4,Dead=5,Guarding=6,GuardImpact=7,GuardBreak =8, GuardParry =9,
	Assassination = 10, Groggy = 11,
	Max
};

UENUM()
enum class ESubStateType : uint8
{
	None =0, JustGuard=1, Down =2, Airborne =3 , UnBlock = 4, Max
};

UENUM()
enum class EHitResponseType : uint8
{
	Normal = 0,          // 기본 모드
	SuperArmor = 1,    // 슈퍼아머 상태
	AntiAirborne = 2,	
	Invincible = 3,    // 무적 상태
	Max
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EStatusEffect : uint8
{
	None        = 0 UMETA(Hidden),
	Weakened    = 1 << 1,
};
ENUM_CLASS_FLAGS(EStatusEffect);

UCLASS()
class PORTFOLIO_API UCStateEnum : public UObject
{
	GENERATED_BODY()
	
};
