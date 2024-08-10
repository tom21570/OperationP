// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Volibear/OPVolibearAnimInstance.h"

#include "Champion/Volibear/OPVolibear.h"

void UOPVolibearAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	Super::UpdateAnimationProperties(DeltaTime);

	if (Volibear == nullptr)
	{
		Volibear = Cast<AOPVolibear>(TryGetPawnOwner());
	}

	if (Volibear)
	{
		bIsThunderingSmashOn = Volibear->GetbQThunderingSmash();
	}
}

void UOPVolibearAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Volibear = Cast<AOPVolibear>(TryGetPawnOwner());
}
