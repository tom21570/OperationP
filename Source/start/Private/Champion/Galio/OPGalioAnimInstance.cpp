// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Galio/OPGalioAnimInstance.h"

#include "Champion/Galio/OPGalio.h"

void UOPGalioAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	Super::UpdateAnimationProperties(DeltaTime);

	if (Galio == nullptr)
	{
		Galio = Cast<AOPGalio>(TryGetPawnOwner());
	}

	if (Galio)
	{
		bW_IsCharging = Galio->GetbW_Charging();
		bR_IsInAir = Galio->GetbR_IsInAir();
	}
}

void UOPGalioAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Galio = Cast<AOPGalio>(TryGetPawnOwner());
}
