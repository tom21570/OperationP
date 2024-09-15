// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPProjectile.generated.h"

class AOPChampion;
class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class START_API AOPProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOPProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> RootCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> OPProjectileMovementComponent;

	UFUNCTION()
	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UProjectileMovementComponent* GetOPProjectileMovementComponent() const { return OPProjectileMovementComponent; }
};
