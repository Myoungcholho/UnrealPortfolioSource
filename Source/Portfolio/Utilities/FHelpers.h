#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraSystem.h"					// 사용하려면 모듈에 빌드해야함
#include "NiagaraFunctionLibrary.h"
#include "Components/PostProcessComponent.h"
#include "Engine/PostProcessVolume.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Portfolio/Components/CStateComponent.h"

#define CheckTrue(x) { if(x == true) return; }
#define CheckTrueResult(x, y) { if(x == true) return y; }

#define CheckFalse(x) { if(x == false) return;}
#define CheckFalseResult(x, y) { if(x == false) return y;}

#define CheckNull(x) { if(x == nullptr) return; }
#define CheckNullResult(x, y) { if(x == nullptr) return y;}

#define CreateTextRenderComponent()\
{\
	CHelpers::CreateComponent<UTextRenderComponent>(this,&Text,"Text",Root);\
	Text->SetRelativeLocation(FVector(0,0,100));\
	Text->SetRelativeRotation(FRotator(0,180,0));\
	Text->SetRelativeScale3D(FVector(2));\
	Text->TextRenderColor = FColor::Red;\
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;\
	Text->Text = FText::FromString(GetName().Replace(L"Default__",L""));\
}


class PORTFOLIO_API FHelpers
{
public:
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr,FName InSocketName = NAME_None)
	{
		*OutComponent =  InActor->CreateDefaultSubobject<T>(InName);

		if(!!InParent)
		{
			(*OutComponent)->SetupAttachment(InParent,InSocketName);

			return;
		}

		InActor->SetRootComponent(*OutComponent);
	}

	template<typename T>	
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent =  InActor->CreateDefaultSubobject<T>(InName);
	}
	
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject,FString InPath)
	{
		*OutObject = Cast<T>(StaticLoadObject(T::StaticClass(),nullptr,*InPath));
	}

	
	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		*OutClass = asset.Class;
	}

	template<typename T>
	static T* FindActor(UWorld* InWorld)
	{
		for(AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if(!!actor && actor->IsA<T>())
				return Cast<T>(actor);
		}

		return nullptr;
	}

	
	template<typename T>
	static void FindActors(UWorld* InWorld,TArray<T*>& OutActors)
	{
		for(AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if(!!actor && actor->IsA<T>())
				OutActors.Add(Cast<T>(actor));
		}
	}

	
	static void AttachTo(AActor* InActor, USceneComponent* InParent, FName InSocketName)
	{
		InActor->AttachToComponent(InParent,FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),InSocketName);
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
	
	template<typename T>
	static T* GetComponent(AActor* InActor,const FString& InName)
	{
		TArray<T*> components;
		InActor->GetComponents<T>(components);

		for(T* component : components)
		{
			if(component->GetName() == InName)
				return component;
		}
		return nullptr;
	}

	static void PlayEffect(UWorld* InWorld, UFXSystemAsset* InAsset, const FTransform& InTransform, USceneComponent* InAttach = nullptr,FName InSocketName = NAME_None)
	{
		UParticleSystem* particle = Cast<UParticleSystem>(InAsset);
		UNiagaraSystem* niagara = Cast<UNiagaraSystem>(InAsset);

		FVector location = InTransform.GetLocation();
		FRotator rotation = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();

		if(!!InAttach)
		{
			if(!!particle)
			{
				UGameplayStatics::SpawnEmitterAttached(particle,InAttach,InSocketName,location,rotation,scale);

				return;
			}

			if(!!niagara)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(niagara,InAttach,InSocketName,location,rotation,scale,EAttachLocation::KeepRelativeOffset,true,ENCPoolMethod::None);
			}
		}

		if(!!particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(InWorld,particle,InTransform);

			return;
		}

		if(!!niagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld,niagara,location,rotation,scale);
		}
	}

	static void SetDefaultState(ACharacter* InCharacter)
	{
		CheckNull(InCharacter);
		UCStateComponent* state = GetComponent<UCStateComponent>(InCharacter);
		CheckNull(state);

		state->SetIdleMode();
		state->SetSubNoneMode();
		state->SetNormalMode();
	}

	static void ApplySlowMotion(UWorld* InWorld, float SlowRate, float Duration, TArray<ACharacter*> ExcludedCharacters)
	{
		CheckNull(InWorld);

		// 현재 레벨에 있는 모든 캐릭터 찾기
		for (TActorIterator<ACharacter> It(InWorld); It; ++It)
		{
			ACharacter* Character = *It;
			if (!Character) continue;

			// 제외할 캐릭터 배열에 포함되어 있다면 넘어감
			if (ExcludedCharacters.Contains(Character))
				continue;

			// 캐릭터 속도를 SlowRate로 설정
			Character->CustomTimeDilation = SlowRate;
		}

		// 일정 시간 후 원래 속도로 복구
		FTimerHandle RestoreTimer;
		InWorld->GetTimerManager().SetTimer(RestoreTimer, [InWorld, ExcludedCharacters]()
		{
			for (TActorIterator<ACharacter> It(InWorld); It; ++It)
			{
				ACharacter* Character = *It;
				if (!Character) continue;

				// 제외한 캐릭터는 건너뜀
				if (ExcludedCharacters.Contains(Character))
					continue;

				// 원래 속도로 복구
				Character->CustomTimeDilation = 1.0f;
			}
		}, Duration, false);
	}

	// 카메라 Transform 기준 절두체 꼭짓점 생성
	static void GenerateFrustumCorners(const FVector& CameraLocation, const FVector& Forward, const FVector& Right, const FVector& Up, 
                                   float Near, float Far, float HalfFOV, float AspectRatio, FVector OutCorners[8])
	{
		// Near/Far 높이/폭 계산
		float NearHeight = 2.0f * FMath::Tan(HalfFOV) * Near;
		float NearWidth  = NearHeight * AspectRatio;
		float FarHeight  = 2.0f * FMath::Tan(HalfFOV) * Far;
		float FarWidth   = FarHeight * AspectRatio;

		// Near/Far 중심
		FVector NearCenter = CameraLocation + Forward * Near;
		FVector FarCenter = CameraLocation + Forward * Far;

		// Near plane 꼭짓점
		OutCorners[0] = NearCenter + (Up * (NearHeight * 0.5f)) - (Right * (NearWidth * 0.5f)); // Near Top Left
		OutCorners[1] = NearCenter + (Up * (NearHeight * 0.5f)) + (Right * (NearWidth * 0.5f)); // Near Top Right
		OutCorners[2] = NearCenter - (Up * (NearHeight * 0.5f)) - (Right * (NearWidth * 0.5f)); // Near Bottom Left
		OutCorners[3] = NearCenter - (Up * (NearHeight * 0.5f)) + (Right * (NearWidth * 0.5f)); // Near Bottom Right

		// Far plane 꼭짓점
		OutCorners[4] = FarCenter + (Up * (FarHeight * 0.5f)) - (Right * (FarWidth * 0.5f)); // Far Top Left
		OutCorners[5] = FarCenter + (Up * (FarHeight * 0.5f)) + (Right * (FarWidth * 0.5f)); // Far Top Right
		OutCorners[6] = FarCenter - (Up * (FarHeight * 0.5f)) - (Right * (FarWidth * 0.5f)); // Far Bottom Left
		OutCorners[7] = FarCenter - (Up * (FarHeight * 0.5f)) + (Right * (FarWidth * 0.5f)); // Far Bottom Right
	}

	// 꼭짓점으로 디버그로 절두체 그리기
	static void DrawFrustumFromCorners(UWorld* World, const FVector Corners[8], FColor Color = FColor::Green, float Duration = 0.5f)
	{
		if (!World) return;

		// Near plane
		DrawDebugLine(World, Corners[0], Corners[1], Color, false, Duration, 0, 2.f);
		DrawDebugLine(World, Corners[1], Corners[3], Color, false, Duration, 0, 2.f);
		DrawDebugLine(World, Corners[3], Corners[2], Color, false, Duration, 0, 2.f);
		DrawDebugLine(World, Corners[2], Corners[0], Color, false, Duration, 0, 2.f);

		// Far plane
		DrawDebugLine(World, Corners[4], Corners[5], Color, false, Duration, 0, 2.f);
		DrawDebugLine(World, Corners[5], Corners[7], Color, false, Duration, 0, 2.f);
		DrawDebugLine(World, Corners[7], Corners[6], Color, false, Duration, 0, 2.f);
		DrawDebugLine(World, Corners[6], Corners[4], Color, false, Duration, 0, 2.f);

		// Connect near and far
		DrawDebugLine(World, Corners[0], Corners[4], Color, false, Duration, 0, 2.f);
		DrawDebugLine(World, Corners[1], Corners[5], Color, false, Duration, 0, 2.f);
		DrawDebugLine(World, Corners[2], Corners[6], Color, false, Duration, 0, 2.f);
		DrawDebugLine(World, Corners[3], Corners[7], Color, false, Duration, 0, 2.f);
	}

	// Transform과 BoxExtent를 받아서 AABB 꼭짓점 생성
	static void GetAABBCornersFromTransform(const FTransform& Transform, const FVector& BoxExtent, FVector OutCorners[8])
	{
		FVector Center = Transform.GetLocation();
		FVector X = Transform.GetUnitAxis(EAxis::X) * BoxExtent.X;
		FVector Y = Transform.GetUnitAxis(EAxis::Y) * BoxExtent.Y;
		FVector Z = Transform.GetUnitAxis(EAxis::Z) * BoxExtent.Z;

		OutCorners[0] = Center + X + Y + Z;
		OutCorners[1] = Center + X + Y - Z;
		OutCorners[2] = Center + X - Y + Z;
		OutCorners[3] = Center + X - Y - Z;
		OutCorners[4] = Center - X + Y + Z;
		OutCorners[5] = Center - X + Y - Z;
		OutCorners[6] = Center - X - Y + Z;
		OutCorners[7] = Center - X - Y - Z;
	}

	// Transform 기준 AABB가 Frustum 안에 하나라도 들어오면 true
	static bool IsAABBIntersectingFrustum(const FTransform& Transform, const FVector& BoxExtent, const FPlane FrustumPlanes[6])
	{
		FVector AABBCorners[8];
		GetAABBCornersFromTransform(Transform, BoxExtent, AABBCorners);

		for (int32 j = 0; j < 8; ++j) // 8개 꼭짓점
		{
			bool bInsideAllPlanes = true;

			for (int32 i = 0; i < 6; ++i) // 6개 평면
			{
				const FPlane& Plane = FrustumPlanes[i];
				float Distance = Plane.PlaneDot(AABBCorners[j]); // (Normal dot Point) + D

				if (Distance > 0.f)
				{
					// 이 점은 이 평면 기준 바깥쪽 (Outside)
					bInsideAllPlanes = false;
					break;
				}
			}

			if (bInsideAllPlanes)
			{
				// 이 꼭짓점은 절두체 안에 있다
				return true;
			}
		}

		// 8개 점 다 돌았는데 하나도 안에 안 들어왔으면
		return false;
	}
};