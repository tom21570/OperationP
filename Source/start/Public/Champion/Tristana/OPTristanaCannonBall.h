// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPTristanaCannonBall.generated.h"

/**
 * 
 */
UCLASS()
class START_API AOPTristanaCannonBall : public AOPProjectile
{
	GENERATED_BODY()
	
protected:
	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_InitialSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_AngleOfFloating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Force;
};

