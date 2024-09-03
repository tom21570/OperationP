// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPBTTask_IncrementPathIndex.h"
#include "AI/OPAIController.h"
#include "AI/OPAIEnemy.h"
#include "AI/PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"

UOPBTTask_IncrementPathIndex::UOPBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "IncrementPathIndex";
}

EBTNodeResult::Type UOPBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto* const Controller = Cast<AOPAIController>(OwnerComp.GetAIOwner()))
	{
		if (const auto* const AIEnemy = Cast<AOPAIEnemy>(Controller->GetPawn()))
		{
			if (auto* const BlackboardComponent = OwnerComp.GetBlackboardComponent())
			{
				// auto const NoOfPoints = AIEnemy->GetPatrolPath()->Num();
				auto const MinIndex = 0;
				// auto const MaxIndex = NoOfPoints - 1;
				auto Index = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());

				if (bBiDirectional)
				{
					// if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					// else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				// BlackboardComponent->SetValueAsInt(GetSelectedBlackboardKey(), (Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
