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
	// Function to initialize the explosive charge
	void InitializeCharge(AActor* TargetActor, float Damage, float ExplosionRadius, float TimeToExplode);

	// Function to apply damage and handle explosion
	void Explode();

	// Function to handle hit and increment hit count
	void OnHit();

private:

	//skill Explosive Charge
// Variables to store the target, damage, radius, and time to explode
	AActor* ExplosiveCharge_Target;
	float ExplosiveCharge_Damage;
	float ExplosiveCharge_ExplosionRadius;
	float ExplosiveCharge_TimeToExplode;
	int32 ExplosiveCharge_HitCount = 0;
	int32 ExplosiveCharge_RequiredHits = 3;

	// Timer handle for explosion
	FTimerHandle ExplosiveCharge_ExplosionTimerHandle;
};
