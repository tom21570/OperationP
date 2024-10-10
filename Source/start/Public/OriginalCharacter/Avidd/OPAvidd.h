// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OriginalCharacter/OPOriginalCharacter.h"
#include "OPAvidd.generated.h"

class AOPPykeHarpoon;
/**
 * 
 */
UCLASS()
class START_API AOPAvidd : public AOPOriginalCharacter
{
	GENERATED_BODY()

public:
	AOPAvidd();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void Skill() override;
	void Pyke_Q();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> HeadMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pyke", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PykeHarpoonMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pyke", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> PykeHarpoonSpawnPoint;

	TObjectPtr<AOPPykeHarpoon> PykeHarpoonStorage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pyke", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPPykeHarpoon> PykeHarpoonClass;

	FRotator PykeHarpoonSpawnRotation = FRotator::ZeroRotator;

	FTimerHandle PykeHarpoonThrowTimer;
	FTimerHandle PykeHarpoonVisibleTimer;
};
