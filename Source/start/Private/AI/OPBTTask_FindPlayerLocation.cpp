// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPBTTask_FindPlayerLocation.h"
#include "NavigationSystem.h"
#include "AI/OPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UOPBTTask_FindPlayerLocation::UOPBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "Find Player Location";
}

EBTNodeResult::Type UOPBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto* const Player  = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		auto const PlayerLocation = Player->GetActorLocation();
		if (SearchRandom)
		{
			FNavLocation Location;

			if (const auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Location))
				{
					auto* const Controller  = Cast<AOPAIController>(OwnerComp.GetAIOwner());
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
					UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerLocation);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			auto* const Controller  = Cast<AOPAIController>(OwnerComp.GetAIOwner());
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
