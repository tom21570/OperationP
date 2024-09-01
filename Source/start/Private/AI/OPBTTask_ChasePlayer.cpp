// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPBTTask_ChasePlayer.h"

#include "NavigationSystem.h"
#include "AI/OPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Slate/SGameLayerManager.h"

UOPBTTask_ChasePlayer::UOPBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UOPBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller  = Cast<AOPAIController>(OwnerComp.GetAIOwner()))
	{
		auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerLocation);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
