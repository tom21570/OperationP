// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPBTTask_MeleeAttack.h"

#include "AIController.h"
#include "AI/OPAIEnemy.h"
#include "Animation/OPAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Champion/OPChampion.h"
#include "Kismet/GameplayStatics.h"
#include "OriginalCharacter/Avidd/OPAvidd.h"

UOPBTTask_MeleeAttack::UOPBTTask_MeleeAttack()
{
	NodeName = "Melee Attack";
}

EBTNodeResult::Type UOPBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	// if (OutOfRange)
	// {
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// 	return EBTNodeResult::Succeeded;
	// }

	auto const* const Controller = OwnerComp.GetAIOwner();
	const auto* const Player  = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto* const Avidd = Cast<AOPAvidd>(Controller->GetPawn());

	// if (MontageHasFinished(Avidd))
	// {
		Avidd->Farm();
	// }
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UOPBTTask_MeleeAttack::MontageHasFinished(AOPAvidd* const Avidd)
{
	return Avidd->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Avidd->GetNasus_Q_AnimMontage());
}
