#include "../Notifies/CAnimNotify_TargetSpawnNiagara.h"

#include "NiagaraComponent.h"
#include "../Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Portfolio/Enemies/CAIController.h"
#include "Portfolio/Enemies/CEnemy_AI.h"

FString UCAnimNotify_TargetSpawnNiagara::GetNotifyName_Implementation() const
{
	return "TargetSpawnEffect";
}

void UCAnimNotify_TargetSpawnNiagara::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(NiagaraEffect);

	// 1. 이 노티파이를 실행한 보스 캐릭터
	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	// 2. 보스의 컨트롤러 (AIController)
	AAIController* AICon = Cast<AAIController>(OwnerActor->GetInstigatorController());
	CheckNull(AICon);

	// 3. 블랙보드에서 Target 가져오기
	UBlackboardComponent* BB = AICon->GetBlackboardComponent();
	CheckNull(BB);

	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("Target"));
	CheckNull(TargetActor);

	UNiagaraComponent* comp = FHelpers::GetComponent<UNiagaraComponent>(TargetActor);
	CheckNull(comp);

	comp->SetAsset(NiagaraEffect);
	comp->Activate(true);
}
