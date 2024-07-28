// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/OPAnimInstance.h"
#include "OPTristanaAnimInstance.generated.h"

class AOPTristana;
/**
 * 
 */
UCLASS()
class START_API UOPTristanaAnimInstance : public UOPAnimInstance
{
	GENERATED_BODY()

public:
	UOPTristanaAnimInstance();

protected:
	// UFUNCTION(BlueprintCallable)
	// virtual void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

private:
	TObjectPtr<AOPTristana> Tristana;
};
