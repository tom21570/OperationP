// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPSkeletonKnightSpear.generated.h"

/**
 * 
 */
UCLASS()
class START_API AOPSkeletonKnightSpear : public AOPProjectile
{
	GENERATED_BODY()

	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	
};
