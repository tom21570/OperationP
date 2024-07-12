// Fill out your copyright notice in the Description page of Project Settings.


#include "Diavolo/OPDiavolo.h"
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

void AOPDiavolo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AOPDiavolo::ApplySlowEffect(float SlowAmount, float Duration)
{
    OriginalSpeed = GetCharacterMovement()->MaxWalkSpeed;
    GetCharacterMovement()->MaxWalkSpeed *= SlowAmount;

    GetWorldTimerManager().SetTimer(SlowEffectTimerHandle, this, &AOPDiavolo::ResetSpeed, Duration, false);
}

void AOPDiavolo::ResetSpeed()
{
    GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
}

USkeletalMeshComponent* AOPDiavolo::GetDiavoloMesh() const
{
    return GetMesh();
}