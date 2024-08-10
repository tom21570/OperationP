// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/OPAnimInstance.h"
#include "OPVolibearAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class START_API UOPVolibearAnimInstance : public UOPAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AOPVolibear* Volibear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsThunderingSmashOn;
};
