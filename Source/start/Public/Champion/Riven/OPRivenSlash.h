// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPRivenSlash.generated.h"

/**
 * 
 */
UCLASS()
class START_API AOPRivenSlash : public AOPProjectile
{
	GENERATED_BODY()

public:
	AOPRivenSlash();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

protected:
	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Slash_Strength = 0.f;
};
