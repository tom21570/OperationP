// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPBTTask_FindPathPoint.h"
#include "AI/OPAIController.h"
#include "AI/PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Champion/OPChampion.h"

UOPBTTask_FindPathPoint::UOPBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "FindPathPoint";
}

EBTNodeResult::Type UOPBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<AOPAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const BlackboardComponent = OwnerComp.GetBlackboardComponent())
		{
			auto const Index = BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey());

			if (auto* AIEnemy = Cast<AOPChampion>(Controller->GetPawn()))
			{
				auto const Point = AIEnemy->GetPatrolPath()->GetPatrolPoint(Index);

				auto const GlobalPoint = AIEnemy->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				BlackboardComponent->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
