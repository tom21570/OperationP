// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPBTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "AI/OPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UOPBTTask_FindRandomLocation::UOPBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UOPBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AOPAIController* const Controller = Cast<AOPAIController>(OwnerComp.GetAIOwner()))
	{
		if (const auto* const AIEnemy = Controller->GetPawn())
		{
			const FVector Origin = AIEnemy->GetActorLocation();

			if (const auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Location;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
				}
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
