// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPTristanaExplosiveCharge.generated.h"

/**
 * 
 */
UCLASS()
class START_API AOPTristanaExplosiveCharge : public AOPProjectile
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Function to apply damage and handle explosion
	void Explode();

private:
	// Timer handle for explosion
	FTimerHandle ExplosiveCharge_ExplosionTimerHandle;

	FTimerHandle DecreaseVelocityTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E", meta = (AllowPrivateAccess = "true"))
	float E_ExplosionRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E", meta = (AllowPrivateAccess = "true"))
	float E_KnockbackStrength = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E", meta = (AllowPrivateAccess = "true"))
	float E_Angle = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E", meta = (AllowPrivateAccess = "true"))
	float E_TimeToExplode = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ExplodeEffect;
};
