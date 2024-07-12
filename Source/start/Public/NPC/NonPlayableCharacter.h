// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NonPlayableCharacter.generated.h"

UCLASS()
class START_API ANonPlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANonPlayableCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void ThrowProjectile();

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AOPProjectile> ProjectileClass;

	FTimerHandle TimerHandle;

};
