#include "../Weapons/CWeaponAsset.h"
#include "../Global.h"

#include "CAttachment.h"
#include "CEquipment.h"
///////////////////////////////////////
#include "DoActions/CDoAction.h"
#include "GameFramework/Character.h"
#include "../Weapons/CWeaponData.h"
#include "Assassinations/CAssassination.h"
#include "CommandActions/CCommandActions.h"
#include "SubActions/CSubAction.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
	DoActionClass = UCDoAction::StaticClass();
	SubActionClass = UCSubAction::StaticClass();
	AssassinationClass = UCAssassination::StaticClass();
}

// WeaponComponent가 호출 보장함.
void UCWeaponAsset::BeginPlay(class ACharacter* InOwner,class UCWeaponData** OutData)
{
	ACAttachment* attachment = nullptr;
	UCEquipment* equipment = nullptr;
	UCDoAction* doAction = nullptr;
	UCSubAction* subAction = nullptr;
	UCAssassination* assassination = nullptr;
	
	// 무기 기본 정보
	if(!!AttachmentClass)
	{
		FActorSpawnParameters Param;
		Param.Owner = InOwner;

		attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass,Param);
	}
	// 무기 장착
	if(!!EquipmentClass)
	{
		equipment = NewObject<UCEquipment>(this,EquipmentClass);
		equipment->BeginPlay(InOwner,EquipData);

		if(!!attachment)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(attachment,&ACAttachment::OnBeginEquip);
			equipment->OnEquipmentUnEquip.AddDynamic(attachment,&ACAttachment::OnUnEquip);
		}
	}

	// 공격
	if(!!DoActionClass)
	{
		// 해당 클래스 생성 및 클래스 정보 할당
		doAction = NewObject<UCDoAction>(this,DoActionClass);
		doAction->BeginPlay(InOwner, attachment, equipment,DoActionDatas,HitDatas,
			CommandActionDatas,CommandHitDatas,DoAirActionDatas,HitAirDatas);

		//무기 정보에 공격 데이터를 등록 및 함수호출 바인딩
		if(!!attachment)
		{
			// 무기 충돌체 꺼지고 켜진경우, doAction 함수 호출
			// 이유 ?
			attachment->OnAttachmentBeginCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentEndCollision);

			// 무기 충돌체가 오버랩된 경우 DoAction에 전달해 처리
			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction,&UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction,&UCDoAction::OnAttachmentEndOverlap);
		}

		//무기 장착 정보가 있다면, 장착할때 하지 않을때 doAction 함수 호출 바인딩
		if(!!equipment)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(doAction, &UCDoAction::OnBeginEquip);
			equipment->OnEquipmentBeginEquip.AddDynamic(doAction, &UCDoAction::OnEndEquip);
			equipment->OnEquipmentUnEquip.AddDynamic(doAction, &UCDoAction::OnUnequip);
		}
	}

	// SubAction (우클릭)
	// 아 카타나 데이터를 여기서 등록해서 넘겨주는게 아니라
	// 본인이 가지고 있는데, 구조가 이게 맞는지 확인 필요함
	if(!!SubActionClass)
	{
		subAction = NewObject<UCSubAction>(this,SubActionClass);
		subAction->BeginPlay(InOwner,attachment,doAction);

		if(!!equipment)
		{
			equipment->OnEquipmentUnEquip.AddDynamic(subAction,&UCSubAction::OnUnEquip);
		}
	}

	// Assassination
	if(!!AssassinationClass)
	{
		assassination = NewObject<UCAssassination>(this,AssassinationClass);
		assassination->BeginPlay(InOwner, AssassinationDatas);
		//  나중에 Detection actor component 안쓰고 싶으면 여기 Tick추가.
	}
	
	
	*OutData = NewObject<UCWeaponData>();
	(*OutData)->WeaponType = WeaponType;
	(*OutData)->Attachment = attachment;
	(*OutData)->Equipment = equipment;
	(*OutData)->DoAction = doAction;
	(*OutData)->SubAction = subAction;
	(*OutData)->Assassination = assassination;
}
