// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "Interface/OPCombatInterface.h"
#include "OPAIEnemy.generated.h"

class AOPSkeletonKnightSpear;
class APatrolPath;
class UBehaviorTree;
class UAIPerceptionStimuliSourceComponent;
class AAIController;
/**
 * 
 */
UCLASS()
class START_API AOPAIEnemy : public AOPChampion, public IOPCombatInterface
{
	GENERATED_BODY()
	
public:
    AOPAIEnemy();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BasicAttack() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> Tree;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<APatrolPath> PatrolPath;

	// TObjectPtr<UAIPerceptionStimuliSourceComponent> StimulusSource;

	// void SetupStimulusSource();
	void ThrowSpear();

private:
	TObjectPtr<AOPSkeletonKnightSpear> SpearStorage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPSkeletonKnightSpear> SpearClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SpearSpawnPoint;

	FTimerHandle SpearThrow_TimerHandle;

public:
	FORCEINLINE TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return Tree; }
	// FORCEINLINE TObjectPtr<APatrolPath> GetPatrolPath() const { return PatrolPath; }
	FORCEINLINE void DeleteTree() { Tree = nullptr; }
	// FORCEINLINE void BasicAttack_Public() { BasicAttack(); }

	int MeleeAttack_AI_Implementation() override;

	void BasicAttack_Public();
};
