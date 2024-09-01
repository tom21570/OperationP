// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPAIEnemy.h"
#include "AIController.h"
#include "AITypes.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AOPAIEnemy::AOPAIEnemy()
{
	AIController = Cast<AAIController>(GetController());
}

void AOPAIEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AOPChampion* Champion = Cast<AOPChampion>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Champion);
	MoveRequest.SetAcceptanceRadius(10000.f);
	
	FNavPathSharedPtr NavPath;
	if (AIController)
	{
		AIController->MoveTo(MoveRequest, &NavPath);
	}
}

void AOPAIEnemy::Tick(float DeltaSeconds)
{

}
