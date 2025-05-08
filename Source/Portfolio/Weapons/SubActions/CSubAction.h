#pragma once

#include "CoreMinimal.h"
#include "CSubAction.generated.h"

UCLASS()
class PORTFOLIO_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction(){return bInAction;}
	
public:
	UCSubAction();
	~UCSubAction();

	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);
	virtual void Tick(float InDeltaTime){}

public:
	virtual void Pressed();
	virtual void Released();

public:
	virtual void Begin_SubAction(){}
	virtual void End_SubAction(){}
	virtual void Begin_UnSubAction(){}
	virtual void End_UnSubAction(){}
	
public:
	UFUNCTION()
	virtual void OnUnEquip(){}
	
protected:
	bool bInAction;

	class ACharacter* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMoveComponent* Movement;
	
};
