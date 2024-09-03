// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPBTService_ChangeSpeed.h"

#include "AIController.h"
#include "AI/OPAIEnemy.h"
#include "Champion/OPChampion.h"
#include "GameFramework/CharacterMovementComponent.h"

UOPBTService_ChangeSpeed::UOPBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = "ChangeSpeed";
}

void UOPBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (const auto* const Controller = OwnerComp.GetAIOwner())
	{
		if (const auto* const AIEnemy = Cast<AOPAIEnemy>(Controller->GetPawn()))
		{
			AIEnemy->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}
