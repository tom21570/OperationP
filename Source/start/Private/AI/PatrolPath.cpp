// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolPath.h"

#include <filesystem>

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector APatrolPath::GetPatrolPoint(int const index) const
{
	return PatrolPoints[index];
}

int APatrolPath::Num() const
{
	return PatrolPoints.Num();
}

// Called when the game starts or when spawned
void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

