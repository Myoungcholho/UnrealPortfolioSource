#include "../Components/CDetectionComponent.h"
#include "../Global.h"
#include "Components/WidgetComponent.h"
#include "Portfolio/Widget/CAssassinationTargetWidget.h"

//#define CDetectionComponent_DEBUG

UCDetectionComponent::UCDetectionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ACharacter* ownerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(ownerCharacter);
	
	FVector Start = ownerCharacter->GetActorLocation();
	FVector End = Start + (ownerCharacter->GetActorForwardVector() * AssassinationRange);
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(ownerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel
	(
		HitResult, Start, End, ECC_Pawn, Params
	);


#ifdef CDetectionComponent_DEBUG
	FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 0, 0, 2.0f);
#endif
	
	if (bHit == false)
	{
		ChangeTarget(nullptr);
		return;
	}
	
	// 감지한 액터가 암살 조건에 맞는지 확인
	AActor* ResultActor = DetectAssassinationTarget(HitResult.GetActor());

	// 조건을 필터한 것으로 교체함
	ChangeTarget(ResultActor);
}

AActor* UCDetectionComponent::DetectAssassinationTarget(AActor* InActor)
{
	CheckNullResult(InActor,nullptr);
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNullResult(OwnerCharacter, nullptr);
	
	float Dot = FVector::DotProduct(InActor->GetActorForwardVector(), OwnerCharacter->GetActorForwardVector());
	bool bIsBehind = (Dot > AssassinationDotThreshold);
	CheckFalseResult(bIsBehind, nullptr);
	
	float Distance = FVector::Dist(OwnerCharacter->GetActorLocation(), InActor->GetActorLocation());
	bool bIsInRange = (Distance < AssassinationRange);
	CheckFalseResult(bIsInRange, nullptr);
	
	return InActor;
}

void UCDetectionComponent::ChangeTarget(AActor* InActor)
{
	CheckNull(TargetWidgetClass);

	// 변경할 엑터가 기존과 같다면 return
	if(CurrentDetectionActor == InActor)
		return;
	
	if (TargetWidget)
	{
		TargetWidget->UnregisterComponent(); // 월드에서 제거
		TargetWidget->DestroyComponent();    // 메모리에서 제거
		TargetWidget = nullptr;
	}
	
	CurrentDetectionActor = InActor;


	if(InActor == nullptr)
		return;

	
	TargetWidget = NewObject<UWidgetComponent>(this,TargetWidgetClass);

	if (TargetWidget)
	{
		TargetWidget->RegisterComponent(); // 동적 생성하면 기본 비활성이기때문에 반드시 호출
		TargetWidget->AttachToComponent(InActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}

AActor* UCDetectionComponent::DetectFrontTarget(float DetectionRange)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNullResult(OwnerCharacter, nullptr);

	// 감지할 범위 설정
	FVector Start = OwnerCharacter->GetActorLocation();
	FVector End = Start + (OwnerCharacter->GetActorForwardVector() * DetectionRange);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params);

#ifdef CDetectionComponent_DEBUG
	FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 0, 0, 2.0f);
#endif

	// 타겟이 감지되지 않으면 nullptr 반환
	if (!bHit) return nullptr;
	
	// 감지된 액터 반환
	return HitResult.GetActor();
}