// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPLeeSinSonicWave.generated.h"

/**
 * 
 */
UCLASS()
class START_API AOPLeeSinSonicWave : public AOPProjectile
{
	GENERATED_BODY()
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void TraceStaticMesh();

	void TraceLeeSin();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float InitialSpeed = 1800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AM_Diavolo_damaged;

	bool bStaticMeshTraceSucceeded = false;
};
