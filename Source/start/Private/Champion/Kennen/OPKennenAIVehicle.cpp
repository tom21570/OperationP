// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Kennen/OPKennenAIVehicle.h"

float AOPKennenAIVehicle::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	TakenVolt = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentVolt += TakenVolt;
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Charge");
	return TakenVolt;
}
