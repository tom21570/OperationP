// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/OPAnimInstance.h"
#include "OPZacAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class START_API UOPZacAnimInstance : public UOPAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void UpdateAnimationProperties(float DeltaTime) override;

	virtual void NativeInitializeAnimation() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AOPZac* Zac;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bEIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bRIsInAir;
};
