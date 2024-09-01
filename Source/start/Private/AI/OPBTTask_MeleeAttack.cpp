// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPBTTask_MeleeAttack.h"

#include "AIController.h"
#include "Animation/OPAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Champion/OPChampion.h"

UOPBTTask_MeleeAttack::UOPBTTask_MeleeAttack()
{
	NodeName = "Melee Attack";
}

EBTNodeResult::Type UOPBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	auto const* const Controller = OwnerComp.GetAIOwner();
	auto* const AIEnemy = Cast<AOPChampion>(Controller->GetPawn());

	if (MontageHasFinished(AIEnemy))
	{
		AIEnemy->GetChampionAnimInstance()->Montage_Play(AIEnemy->GetBasicAttackAnimMontage());
	}
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UOPBTTask_MeleeAttack::MontageHasFinished(AOPChampion* const Enemy)
{
	return Enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Enemy->GetBasicAttackAnimMontage());
}
