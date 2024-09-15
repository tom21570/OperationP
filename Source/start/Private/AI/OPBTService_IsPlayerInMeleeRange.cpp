// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPBTService_IsPlayerInMeleeRange.h"

#include "AI/OPAIController.h"
#include "AI/OPAIEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Champion/OPChampion.h"
#include "Kismet/GameplayStatics.h"

UOPBTService_IsPlayerInMeleeRange::UOPBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = "IsPlayerInMeleeRange";
}

void UOPBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const* const Controller = Cast<AOPAIController>(OwnerComp.GetAIOwner());
	auto const* const AIEnemy = Cast<AOPAIEnemy>(Controller->GetPawn());

	auto const* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), AIEnemy->GetDistanceTo(Player) <= MeleeRange);
}
