#include "../../Weapons/DoActions/CDoAction_Boss.h"
#include "../../Global.h"

void UCDoAction_Boss::DoAction(int32 InPlayIndex)
{
	CheckFalse(DoActionDatas.Num() > InPlayIndex);

	CheckFalse(State->IsIdleMode());
	Super::DoAction(InPlayIndex);

	Index = InPlayIndex;
	DoActionDatas[InPlayIndex].DoAction(OwnerCharacter);
}

void UCDoAction_Boss::End_DoAction()
{
	Super::End_DoAction();
}

void UCDoAction_Boss::OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser,
	class ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);

	for(ACharacter* hitted: Hitted)
	{
		CheckTrue(hitted == InOther);
	}

	Hitted.AddUnique(InOther);
	
	CheckFalse(HitDatas.Num() > Index);
	HitDatas[Index].SendDamage(InAttacker,InAttackCauser,InOther);
}

void UCDoAction_Boss::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();
	
	Hitted.Empty();
}
