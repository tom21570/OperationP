// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Zac/OPZacAnimInstance.h"

#include "Champion/Zac/OPZac.h"

void UOPZacAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	Super::UpdateAnimationProperties(DeltaTime);

	if (Zac == nullptr)
	{
		Zac = Cast<AOPZac>(TryGetPawnOwner());
	}

	if (Zac)
	{
		bEIsInAir = Zac->GetbEIsInAir();
		bRIsInAir = Zac->GetbRIsInAir();
	}
}

void UOPZacAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Zac = Cast<AOPZac>(TryGetPawnOwner());
}
