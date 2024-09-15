// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "OPBTTask_IncrementPathIndex.generated.h"

/**
 * 
 */
UCLASS()
class START_API UOPBTTask_IncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UOPBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	enum class EDirectionType {Forward, Reverse};

	EDirectionType Direction = EDirectionType::Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bBiDirectional = false;
};
