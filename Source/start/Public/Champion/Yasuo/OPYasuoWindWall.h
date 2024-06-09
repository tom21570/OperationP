// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPYasuoWindWall.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class START_API AOPYasuoWindWall : public AOPProjectile
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	AOPYasuoWindWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ProjectileEffect;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraComponent> ProjectileNiagaraComponent;

	FVector StartLocation;

	FTimerHandle StopTimerHandle;
	FTimerHandle DestroyTimerHandle;

	void StopProjectile();
	void DestroyProjectile();

	UFUNCTION()
	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitProjectile(FVector Direction, float Speed);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float InitialSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float DistanceToStop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float StopDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WindWallForce;
};