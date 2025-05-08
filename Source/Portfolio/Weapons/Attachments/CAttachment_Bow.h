#pragma once

#include "CoreMinimal.h"
#include "../../Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"

UCLASS()
class PORTFOLIO_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	class UPoseableMeshComponent* PoseableMesh;

public:
	ACAttachment_Bow();

protected:
	void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	void OnBeginEquip_Implementation() override;
	void OnUnEquip_Implementation() override;

public:
	float* GetAnimInstance_Bend();
};
