#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CCameraManagerComponent.generated.h"

struct FMaterialMemento
{
	TArray<UMaterialInterface*> Materials;

	FMaterialMemento(const UMeshComponent* Mesh)
	{
		for (int i = 0; i < Mesh->GetNumMaterials(); ++i)
			Materials.Add(Mesh->GetMaterial(i));
	}

	void Restore(UMeshComponent* Mesh) const
	{
		for (int i = 0; i < Materials.Num(); ++i)
			Mesh->SetMaterial(i, Materials[i]);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCCameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,Category="ChangeMaterial")
	class UMaterialInstance* TransparentMaterial;

	UPROPERTY(EditAnywhere,Category="CameraBlendDatas")
	class UCCameraBlendAsset* BlendDatas;

	UPROPERTY(EditAnywhere, Category = "Debug")
	FVector BoxHalfSize = FVector(10.f,170.f,30.f);
	
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	float FarPlane = 100.0f;
	
public:	
	UCCameraManagerComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class ACharacter* OwnerCharacter;

private:
	void HandleCameraCollision();
	void MakeActorTransparent(AActor* HitActor);
	void ResetActorMaterial(AActor* Actor);
	
private:
	UPROPERTY()
	class UCameraComponent* Camera;               // 카메라 컴포넌트
	UPROPERTY()
	class USpringArmComponent* SpringArm;         // Spring Arm 컴포넌트

	
	TSet<AActor*> PreviouslyTransparentActors; // 이전 프레임에서 투명화된 오브젝트
	//TMap<AActor*, TArray<UMaterialInterface*>> TransparentMaterials; // 투명화된 오브젝트의 원래 Material
	TMap<AActor*, FMaterialMemento> MaterialMementos;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TSubclassOf<AActor> SubCameraClass;

	UPROPERTY()
	AActor* SubCameraActor;
	
	FTimerHandle CameraResetTimer;
	
public:
	// 보조 카메라 전환 함수
	void ChangeToSubCamera(FName InName);
	void Notify_ChangeToSubCamera(FName InName);
	
	// 원래 카메라로 복귀
	void ResetToMainCamera(float BlendTime);

	// 타겟암 변경
	void StartZoom();
	void ResetZoom();
	
private:
	float OriginalArmLength;
	float ZoomTargetLength = 200.f;
	float ZoomInterpSpeed = 25.f;

	FTimerHandle ZoomResetHandle;
	bool bZoomingOut = false;
	
//디버그용 카메라 전환
public:
	UPROPERTY(EditDefaultsOnly, Category="UnitTest")
	TSubclassOf<class ACCameraMoveTest> UnitTestClass;

	class ACCameraMoveTest* UnitTest1;
	
	// F1 키 누르면 호출
	void ChangeCamera();
	int32 CurrentCameraIndex = 0;
};
