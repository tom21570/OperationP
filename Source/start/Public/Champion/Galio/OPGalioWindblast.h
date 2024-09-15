// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPGalioWindblast.generated.h"

class AOPGalioTornado;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class START_API AOPGalioWindblast : public AOPProjectile
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void WindTrace() const;

	void SpawnTornado(const FVector& Location) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_WindStrength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_TornadoStrength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> Q_TornadoParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPGalioTornado> Q_TornadoClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	FVector Q_Tornado_Size = FVector(1.f, 1.f, 1.f);

	FVector TornadoSpawnLocation;
	FTimerHandle TornadoRemain_TimerHandle;
};
