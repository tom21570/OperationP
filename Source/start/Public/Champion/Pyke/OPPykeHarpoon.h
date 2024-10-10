// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPPykeHarpoon.generated.h"

class UCableComponent;

/**
 * 
 */
UCLASS()
class START_API AOPPykeHarpoon : public AOPProjectile
{
	GENERATED_BODY()

public:
	AOPPykeHarpoon();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	// virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	// 	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void TraceForCharacter();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Pyke", meta = (AllowPrivateAccess = true ))
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Pyke", meta = (AllowPrivateAccess = true ))
	TObjectPtr<UCableComponent> Rope;

	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Pyke", meta = (AllowPrivateAccess = true ))
	float GrabStrength_XY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Pyke", meta = (AllowPrivateAccess = true ))
	float GrabStrength_Z;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Pyke", meta = (AllowPrivateAccess = true ))
	bool IsGrabbedCharacter = false;
};
