// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "OriginalCharacter/Avidd/OPAvidd.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AOPAIController::AOPAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AOPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (const AOPAvidd* const AIAvidd = Cast<AOPAvidd>(InPawn))
	{
		if (UBehaviorTree* const Tree = AIAvidd->GetBehaviorTree())
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(Tree->BlackboardAsset, BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(Tree);
		}
	}
}

void AOPAIController::OnUnPossess()
{
	Super::OnUnPossess();

	SightConfig = nullptr;
	Blackboard = nullptr;
}

void AOPAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component"));
		
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		// GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AOPAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense((*SightConfig));
	}
}

// void AOPAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
// {
// 	if (auto* const OPCharacter = Cast<AOPChampion>(Actor))
// 	{
// 		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
// 	}
// }
