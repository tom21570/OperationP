// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OPAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class START_API UOPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UOPAnimInstance();

protected:
	UFUNCTION(BlueprintCallable)
	virtual void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class AOPChampion* OPChampion;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsDamaged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsStumbledByLeeSinE;
	
	FORCEINLINE float GetSpeed() const { return Speed; }
	FORCEINLINE bool GetbIsAccelerating() const { return bIsAccelerating; }

public:
	FORCEINLINE bool GetbIsInAir() const { return bIsInAir; }
	FORCEINLINE void SetbIsInAir_True() { bIsInAir = true; }
	FORCEINLINE void SetbIsInAir_False() { bIsInAir = false; }
	
};
