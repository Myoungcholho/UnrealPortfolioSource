#include "../../Weapons/SubActions/CSubAction_Aiming.h"
#include "../../Global.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Portfolio/Weapons/Attachments/CAttachment_Bow.h"
#include "Curves/CurveVector.h"
#include "Portfolio/Weapons/DoActions/CDoAction_Bow.h"

UCSubAction_Aiming::UCSubAction_Aiming()
{
	FHelpers::GetAsset<UCurveVector>(&Curve,"/Script/Engine.CurveVector'/Game/Weapons/Bow/Curve_Aiming.Curve_Aiming'");
}

void UCSubAction_Aiming::BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment,
	class UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = FHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = FHelpers::GetComponent<UCameraComponent>(InOwner);
	
	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);

	if (!!bow)
		Bend = bow->GetAnimInstance_Bend();
}

void UCSubAction_Aiming::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);
}

void UCSubAction_Aiming::Pressed()
{
	CheckNull(SpringArm);
	CheckNull(Camera);
	
	Super::Pressed();

	State->OnSubAction();

	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	Timeline.PlayFromStart();

	UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(DoAction);
	CheckNull(bow);

	bow->EnableSubAction();

	if(OnAimingToggle.IsBound())
		OnAimingToggle.Broadcast(true);
}

void UCSubAction_Aiming::Released()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	// Evade로 호출될 수 있음
	if(bInAction == false)
		return;
		
	Super::Released();

	State->OffSubAction();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	Timeline.ReverseFromEnd();

	UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(DoAction);
	CheckNull(bow);

	bow->DisableSubAction();

	if(OnAimingToggle.IsBound())
		OnAimingToggle.Broadcast(false);
}

void UCSubAction_Aiming::OnUnEquip()
{
	CheckFalse(State->IsSubActionMode());

	Released();
}

void UCSubAction_Aiming::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.Z;
	
	if (!!Bend)
		*Bend = Output.Y;
}
