#include "../../Weapons/SubActions/CSubAction.h"
#include "../../Global.h"
#include "GameFramework/Character.h"

#include "Portfolio/Components/CMoveComponent.h"
#include "Portfolio/Components/CStateComponent.h"

UCSubAction::UCSubAction()
{
	
}

UCSubAction::~UCSubAction()
{
	
}

void UCSubAction::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = FHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = Cast<UCMoveComponent>(Owner->GetComponentByClass(UCMoveComponent::StaticClass()));
}

// 마우스 우클릭 눌렸다면 실행
void UCSubAction::Pressed()
{
	bInAction = true;
}

// 마우스 우클릭 땠다면 실행
void UCSubAction::Released()
{
	bInAction = false;
}
