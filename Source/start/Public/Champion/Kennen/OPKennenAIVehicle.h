// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "OPKennenAIVehicle.generated.h"

/**
 * 
 */
UCLASS()
class START_API AOPKennenAIVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float CurrentVolt = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float NecessaryVolt = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float ChargedVoltAtOnce = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bChargeFinished = false;

	float TakenVolt = 0.f;

public:
	FORCEINLINE float GetCurrentVolt() const { return CurrentVolt; }
	FORCEINLINE float GetNecessaryVolt() const { return NecessaryVolt; }

	FORCEINLINE void ChargeFinished() { bChargeFinished = true; }
	void ChargeVolt(const float& Volt) { CurrentVolt += Volt; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
