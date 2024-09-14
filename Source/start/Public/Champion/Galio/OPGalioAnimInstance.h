// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/OPAnimInstance.h"
#include "OPGalioAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class START_API UOPGalioAnimInstance : public UOPAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AOPGalio* Galio;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bW_IsCharging;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bR_IsInAir;
};
