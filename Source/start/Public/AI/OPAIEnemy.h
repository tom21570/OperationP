// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPAIEnemy.generated.h"

class AAIController;
/**
 * 
 */
UCLASS()
class START_API AOPAIEnemy : public AOPChampion
{
	GENERATED_BODY()
	
public:
    AOPAIEnemy();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
	AAIController* AIController;
};
