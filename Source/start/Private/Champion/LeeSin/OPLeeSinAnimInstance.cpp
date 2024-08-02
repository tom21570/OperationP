// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/LeeSin/OPLeeSinAnimInstance.h"

#include "Champion/LeeSin/OPLeeSin.h"

void UOPLeeSinAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	Super::UpdateAnimationProperties(DeltaTime);

	if (LeeSin == nullptr)
	{
		LeeSin = Cast<AOPLeeSin>(TryGetPawnOwner());
	}

	if (LeeSin)
	{
		bIsResonating = LeeSin->GetbIsResonating();
	}
}

void UOPLeeSinAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	LeeSin = Cast<AOPLeeSin>(TryGetPawnOwner());
}
