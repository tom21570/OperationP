// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OPCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOPCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class START_API IOPCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int MeleeAttack_AI();
};
