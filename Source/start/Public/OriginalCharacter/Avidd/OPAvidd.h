// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OriginalCharacter/OPOriginalCharacter.h"
#include "OPAvidd.generated.h"

class AOPPykeHarpoon;
class UBehaviorTree;

UENUM(BlueprintType, Blueprintable)
enum class EAviddType : uint8
{
	EAT_Pyke UMETA(DisplayName = "Pyke"),
	EAT_Nasus UMETA(DisplayName = "Nasus"),
	EAT_Amumu UMETA(DisplayName = "Amumu"),

	EAT_None UMETA(DisplayName = "None"),
};

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
	void Pyke_ThrowHarpoon();
	void Pyke_Q();
	void Nasus_Q();
	void Amumu_W();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> HeadMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avidd Type", meta = (AllowPrivateAccess = "true"))
	EAviddType AviddType = EAviddType::EAT_None;

	//AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> Tree;

	// Pyke
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pyke", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PykeHarpoonMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pyke", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> PykeHarpoonSpawnPoint;

	TObjectPtr<AOPPykeHarpoon> PykeHarpoonStorage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pyke", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPPykeHarpoon> PykeHarpoonClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pyke", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Pyke_Q_AnimMontage;

	FRotator PykeHarpoonSpawnRotation = FRotator::ZeroRotator;

	FTimerHandle PykeHarpoonThrowTimer;
	FTimerHandle PykeHarpoonVisibleTimer;
	
	//Nasus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nasus", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> NasusWeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nasus", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Nasus_Q_AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Amumu", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Amumu_W_AnimMontage;

public:
	FORCEINLINE TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return Tree; }

	UFUNCTION(BlueprintImplementableEvent)
	void Farm();
	TObjectPtr<UAnimMontage> GetNasus_Q_AnimMontage() const { return Nasus_Q_AnimMontage; }
};
