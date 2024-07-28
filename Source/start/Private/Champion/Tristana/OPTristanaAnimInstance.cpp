// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Tristana/OPTristanaAnimInstance.h"

#include "Champion/Tristana/OPTristana.h"

UOPTristanaAnimInstance::UOPTristanaAnimInstance()
{
}

// void UOPTristanaAnimInstance::UpdateAnimationProperties(float DeltaTime)
// {
// 	Super::UpdateAnimationProperties(DeltaTime);
//
// 	if(Tristana == nullptr)
// 	{
// 		Tristana = Cast<AOPTristana>(TryGetPawnOwner());
// 	}
//
// 	if(Tristana)
// 	{
// 		bIsInAir = OPChampion->GetCharacterMovement()->IsFalling();
// 	}
// }

void UOPTristanaAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Tristana = Cast<AOPTristana>(TryGetPawnOwner());
}
