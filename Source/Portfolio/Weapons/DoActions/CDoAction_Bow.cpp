#include "../../Weapons/DoActions/CDoAction_Bow.h"
#include "../../Global.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Portfolio/Weapons/CAttachment.h"
#include "Portfolio/Weapons/CEquipment.h"
#include "Portfolio/Weapons/Addons/CArrow.h"
#include "Portfolio/Weapons/Attachments/CAttachment_Bow.h"

UCDoAction_Bow::UCDoAction_Bow()
{
	FHelpers::GetClass<ACArrow>(&ArrowClass, "/Script/Engine.Blueprint'/Game/Weapons/Bow/BP_CArrow.BP_CArrow_C'");
}

void UCDoAction_Bow::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment,
                               class UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionDatas, const TArray<FDoHitData>& InHitDatas,
                               const TArray<FDoActionData>& InCommandActionDatas, const TArray<FDoHitData>& InCommandHitDatas,
                               const TArray<FDoActionData>& InDoAirActionDatas, const TArray<FDoHitData>& InHitAirDatas)
{
	Super::BeginPlay(InOwner, InAttachment, InEquipment, InDoActionDatas, InHitDatas, InCommandActionDatas,
	                 InCommandHitDatas,
	                 InDoAirActionDatas, InHitAirDatas);
	Attachment = InAttachment;
	bEquipped = InEquipment->GetEquipped();

	SkeletalMesh = FHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
	PoseableMesh = FHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);

	StringOriginLocation = PoseableMesh->GetBoneLocationByName(StringBoneName, EBoneSpaces::ComponentSpace);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	if (!!bow)
		Bend = bow->GetAnimInstance_Bend();

	bAttachedString = false;
}

void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	CheckNull(bEquipped);
	CheckFalse(*bEquipped);
	CheckFalse(bAttachedString);
	CheckTrue(bBeginAction);	// 반쪽짜리인게 콤보1은 적용이 안됨
	
	// 이건 줄(string)임
	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_String");
	PoseableMesh->SetBoneLocationByName(StringBoneName, handLocation, EBoneSpaces::WorldSpace);
}

void UCDoAction_Bow::DoAction()
{
	CheckTrue(DoActionDatas.Num() <1);

	if(bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}
	CheckFalse(State->IsIdleMode());
	Super::DoAction();
	// Zoom 상태라면
	/*if(bAttachedString == true)
	{
		if(!!ZoomDoAction.Montage)
			ZoomDoAction.DoAction(OwnerCharacter);
		return;
	}*/
	
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Bow::Begin_DoAction()
{
	if(State->IsDamagedMode())
		return;
	if(State->IsGuardParryMode())
		return;
	if(State->IsGuardBreakMode())
		return;
	if(State->IsActionMode() == false)
		return;
	
	if(Index + 1 >= DoActionDatas.Num())
		return;

	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;

	//bAttachedString = true;
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	bExist = false;
	Index = 0;
}

void UCDoAction_Bow::ShootArrow()
{
	CreateArrow();
	
	if(!!Bend)
		*Bend = 0.0f;

	//if(bSubAction == false)
		bAttachedString = false;
	
	PoseableMesh->SetBoneLocationByName(StringBoneName, StringOriginLocation, EBoneSpaces::ComponentSpace);
    
	ACArrow* arrow = GetAttachedArrow();
	CheckNull(arrow);

	FDoHitData data;
	data = HitDatas[Index];

	arrow->Init(OwnerCharacter,Attachment,data);
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
	arrow->Shoot(forward);
}

void UCDoAction_Bow::OnEndEquip()
{
	Super::OnEndEquip();
	
}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();
	
	*Bend = 0.0f;
	PoseableMesh->SetBoneLocationByName(StringBoneName, StringOriginLocation, EBoneSpaces::ComponentSpace);

	DestroyArrow();
}

void UCDoAction_Bow::CreateArrow()
{
	CheckTrue(World->bIsTearingDown);
	CheckNull(ArrowClass);
	if(GetAttachedArrow() != nullptr)
		return;
	
	FTransform transform;
	ACArrow* arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass,transform,nullptr,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNull(arrow);
	
	arrow->OnArrowEndPlay.AddDynamic(this,&UCDoAction_Bow::UCDoAction_Bow::OnArrowEndPlay);
	//arrow->OnArrowHit.AddDynamic(this,&UCDoAction_Bow::OnArrowHit);

	arrow->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Hand_Bow_Right_Arrow");
	
	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

void UCDoAction_Bow::DestroyArrow()
{
	ACArrow* arrow = GetAttachedArrow();

	if (!!arrow)
	{
		Arrows.Remove(arrow);
		arrow->Destroy();
	}
}

class ACArrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (ACArrow* arrow : Arrows)
	{
		if (!!arrow && !!arrow->GetAttachParentActor())
			return arrow;
	}

	return nullptr;
}

void UCDoAction_Bow::CanCreateArrow()
{
	//FLog::Log("1");
	if(bAttachedString == true)
	{
		//FLog::Log("2");
		SetAttachedString();
	}
}

// notify call from bow_string + subAction Press
void UCDoAction_Bow::SetAttachedString()
{
	bAttachedString = true;
	*Bend = 100;
	CreateArrow();	
}

void UCDoAction_Bow::LastAttachedString()
{
	if(bSubAction == false)
		return;
	
	bAttachedString = true;
	*Bend = 100;
	CreateArrow();	
}

// subAction Release
void UCDoAction_Bow::ResetAttachedString()
{
	bAttachedString = false; 
	*Bend = 0;
	DestroyArrow();
}

void UCDoAction_Bow::EnableSubAction()
{
	bSubAction = true;
	SetAttachedString();
}

void UCDoAction_Bow::DisableSubAction()
{
	bSubAction = false;
	ResetAttachedString();
}


// 화살이 끝난 경우
void UCDoAction_Bow::OnArrowEndPlay(class ACArrow* InDestroyer)
{
	Arrows.Remove(InDestroyer);
}

// 화살이 누굴 부딪힌 경우
/*void UCDoAction_Bow::OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter)
{
	CheckTrue(HitDatas.IsEmpty());

	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
}*/
