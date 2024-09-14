// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPGalioTornado.generated.h"

/**
 * 
 */
UCLASS()
class START_API AOPGalioTornado : public AOPProjectile
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_TornadoStrength = 0.f;
	
};
