#include "../Weapons/CWeaponStructures.h"
#include "../Global.h"
#include "GameFramework/Character.h"

#include "Portfolio/Components/CMoveComponent.h"

void FDoActionData::DoAction(class ACharacter* InOwner)
{
	UCMoveComponent* movement = FHelpers::GetComponent<UCMoveComponent>(InOwner);

	if(!!movement)
	{
		if(bFixedCamera)
			movement->EnableFixedCamera();
		
		if(bCanMove == false)
			movement->Stop();
	}

	if(!!Montage)
	{
		InOwner->PlayAnimMontage(Montage,PlayRate);
	}
}

///////////////////////////////////////////////////////////////////////////
void FDoHitData::SendDamage(class ACharacter* InAttacker, AActor* InAttackerCauser, class ACharacter* InOther)
{
	FActionDamageEvent e;
	e.HitData = this;

	if(InAttacker == InOther) return;
	
	InOther->TakeDamage(AttackProperty.Power,e,InAttacker->GetController(), InAttackerCauser);
}

/*void FDoHitData::PlayHitMotion(class ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(Montage);

	InCharacter->PlayAnimMontage(Montage,PlayRate);
}*/

void FDoHitData::PlayHitStop(class ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckTrue(FMath::IsNearlyZero(HitEffectData.StopTime));
	
	TArray<AActor*> actors;
	for(AActor* actor : InCharacter->GetWorld()->GetCurrentLevel()->Actors)
	{
		if(actor == nullptr) continue;
		
		UMeshComponent* mesh =  actor->GetComponentByClass<UMeshComponent>();
		if(mesh == nullptr) continue;

		if(mesh->Mobility == EComponentMobility::Movable)
		{
			actor->CustomTimeDilation = 1e-3f;

			actors.Add(actor);
		}
	}
	
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
	{
		for(AActor* actor : actors)
		{
			if(!!actor)
				actor->CustomTimeDilation = 1.0f;
		}
	});

	FTimerHandle timerHandle;
	InCharacter->GetWorld()->GetTimerManager().SetTimer(timerHandle,timerDelegate,HitEffectData.StopTime,false);
}

void FDoHitData::PlayEffect(class ACharacter* InCharacter, class ACharacter* Attacker, EWeaponType InType,FVector* InLocation)
{
	CheckNull(InCharacter);
	CheckNull(HitEffectData.Effect);

	if(InType == EWeaponType::Katana)
	{
		// 피격자의 위치를 기준으로 설정
		FVector location = InCharacter->GetActorLocation();

		// 무기 소켓의 위치를 가져와 궤적 방향 계산
		USkeletalMeshComponent* SkeletalMesh = Attacker->GetMesh();
		CheckNull(SkeletalMesh);

		// 무기 시작점과 끝점 (소켓 이름으로 조정)
		FVector StartPoint = SkeletalMesh->GetSocketLocation("Trail_Start"); // 무기 시작점
		FVector EndPoint = SkeletalMesh->GetSocketLocation("Trail_End");     // 무기 끝점

		// 타격 궤적 방향
		FVector HitDirection = (EndPoint - StartPoint).GetSafeNormal();
		FRotator rotation = HitDirection.Rotation(); // 방향을 회전값으로 변환

		// EffectLocation을 타격 방향에 맞춰 보정
		location += rotation.RotateVector(HitEffectData.EffectLocation);

		// Transform 설정
		FTransform transform;
		transform.SetLocation(location);                     // 피격자의 위치
		transform.SetRotation(rotation.Quaternion());        // 타격 궤적의 방향으로 회전
		
		transform.SetScale3D(HitEffectData.EffectScale);                   // 이펙트 크기
		FHelpers::PlayEffect(InCharacter->GetWorld(),HitEffectData.Effect,transform);
	}
	else if(InType == EWeaponType::Bow)
	{
		FTransform transform;
		transform.SetLocation(*InLocation);
		
		transform.SetScale3D(HitEffectData.EffectScale);
		FHelpers::PlayEffect(InCharacter->GetWorld(), HitEffectData.Effect, transform);
	}
	
}

void FDoHitData::PlaySound(class ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(HitEffectData.Sound);

	UGameplayStatics::SpawnSoundAtLocation(InCharacter->GetWorld(),HitEffectData.Sound,InCharacter->GetActorLocation());
}

void FDoHitData::PlayCameraShake(class ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(HitEffectData.CameraShake);
	
	APlayerCameraManager* manager =  UGameplayStatics::GetPlayerCameraManager(InCharacter->GetWorld(),0);
	CheckNull(manager);

	manager->StartCameraShake(HitEffectData.CameraShake);
}

void FGuardActivateData::PlayBeginGuard(ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	if(!!BeginGuardMontage)
	{
		InCharacter->PlayAnimMontage(BeginGuardMontage,BeginGuard_PlayRate);
	}
}

void FGuardActivateData::PlayEndGuard(ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	if(!!EndGuardMontage)
	{
		InCharacter->PlayAnimMontage(EndGuardMontage,EndGuard_PlayRate);
	}
}

void FGuardBrokenData::PlayBrokenMotion(class ACharacter* InCharacter)
{
	CheckNull(GuardBrokenMontage);
	CheckNull(InCharacter);

	InCharacter->PlayAnimMontage(GuardBrokenMontage,BeginGuard_PlayRate);
}

///////////////////////////////////////////////////////////////////
void FGuardData::PlayGuardMotion(class ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(GuardActionMontage);
	
	InCharacter->PlayAnimMontage(GuardActionMontage,GuardAction_PlayRate);
}

void FGuardData::PlayHitStop(class ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckTrue(FMath::IsNearlyZero(StopTime));

	TArray<AActor*> actors;
	for(AActor* actor : InCharacter->GetWorld()->GetCurrentLevel()->Actors)
	{
		if(actor == nullptr) continue;
		
		UMeshComponent* mesh =  actor->GetComponentByClass<UMeshComponent>();
		if(mesh == nullptr) continue;

		if(mesh->Mobility == EComponentMobility::Movable)
		{
			actor->CustomTimeDilation = 1e-3f;

			actors.Add(actor);
		}
	}

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
	{
		for(AActor* actor : actors)
		{
			if(!!actor)
				actor->CustomTimeDilation = 1.0f;
		}
	});

	FTimerHandle timerHandle;
	InCharacter->GetWorld()->GetTimerManager().SetTimer(timerHandle,timerDelegate,StopTime,false);
}

void FGuardData::PlaySound(class ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(Sound);

	UGameplayStatics::SpawnSoundAtLocation(InCharacter->GetWorld(),Sound,InCharacter->GetActorLocation());
}

void FGuardData::PlayCameraShake(class ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(CameraShake);
	
	APlayerCameraManager* manager =  UGameplayStatics::GetPlayerCameraManager(InCharacter->GetWorld(),0);
	CheckNull(manager);

	manager->StartCameraShake(CameraShake);
}
///////////////////////////////////////////////////////////////////