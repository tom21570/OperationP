// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/OPAnimInstance.h"
#include "Champion/OPChampion.h"
#include "GameFramework/CharacterMovementComponent.h"

UOPAnimInstance::UOPAnimInstance()
{
}

void UOPAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(OPChampion == nullptr)
	{
		OPChampion = Cast<AOPChampion>(TryGetPawnOwner());
	}

	if(OPChampion)
	{
		FVector Velocity = OPChampion->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = OPChampion->GetCharacterMovement()->IsFalling();
		bIsDamaged = OPChampion->GetbIsDamaged();

		if((OPChampion->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0) || Speed > 0.f)
		{
			bIsAccelerating = true;
		}

		else
		{
			bIsAccelerating = false;
		}
	}
}

void UOPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}
