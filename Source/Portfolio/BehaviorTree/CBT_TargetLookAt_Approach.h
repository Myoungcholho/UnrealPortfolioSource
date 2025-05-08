// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BehaviorTree/CBTTaskNode_TargetLookAt.h"
#include "CBT_TargetLookAt_Approach.generated.h"

// 삭제 가능

UCLASS()
class PORTFOLIO_API UCBT_TargetLookAt_Approach : public UCBTTaskNode_TargetLookAt
{
	GENERATED_BODY()
public:
	UCBT_TargetLookAt_Approach();
	
protected:
//	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
