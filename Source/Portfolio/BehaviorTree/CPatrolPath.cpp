#include "../BehaviorTree/CPatrolPath.h"
#include "../Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	bRunConstructionScriptOnDrag = false;

	FHelpers::CreateComponent<USceneComponent>(this,&Root,"Root");
	FHelpers::CreateComponent<USplineComponent>(this,&Spline,"Spline",Root);
	FHelpers::CreateComponent<UTextRenderComponent>(this,&Text,"Text",Root);

	// 땅에 파묻히면 안보이기 때문에..
	Spline->SetRelativeLocation(FVector(0,0,30));

	Text->SetRelativeLocation(FVector(0,0,120));
	Text->SetRelativeRotation(FRotator(0,180,0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
	Text->bHiddenInGame = true;
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	Text->Text = FText::FromString(GetActorLabel());
#endif
	Spline->SetClosedLoop(bLoop);
}

FVector ACPatrolPath::GetMoveTo()
{
	return Spline->GetLocationAtSplinePoint(Index,ESplineCoordinateSpace::World);
}


void ACPatrolPath::UpdateIndex()
{
	int count = Spline->GetNumberOfSplinePoints();

	if(bReverse)
	{
		if(Index > 0 )
		{
			Index--;
			return;
		}

		if(Spline->IsClosedLoop())
		{
			Index = count -1;
			return;
		}
		Index = 1;
		bReverse = false;
		return;
	}
	
	if(Index < count -1)
	{
		Index++;
		return;
	}

	if(Spline->IsClosedLoop())
	{
		Index = 0;
		return;
	}
	Index = count -2;
	bReverse = true;
	
}
