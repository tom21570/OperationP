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

void AOPDiavolo::ApplySlowAttackEffect(float SlowAttackAmount, float Duration)
{
    //To Do
}

void AOPDiavolo::ResetSpeed()
{
    GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
}

USkeletalMeshComponent* AOPDiavolo::GetDiavoloMesh() const
{
    return GetMesh();
}

void AOPDiavolo::PlayDiavoloRandomDeadMontage()
{
    if (GetChampionAnimInstance() == nullptr) return;
    int32 Section = FMath::RandRange(0, 4);
    FTimerHandle DeadTimer;
    GetChampionAnimInstance()->Montage_Play(DeadAnimMontage);
    switch (Section)
    {								
    case 0:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_2"));
        GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
        {
            GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_2"));
        }), 4.45f, false);
        break;
				
    case 1:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_3"));
        GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
        {
            GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_3"));
        }), 3.5f, false);
        break;
				
    case 2:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_4"));
        GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
        {
            GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_4"));
        }), 3.4f, false);
        break;

    case 3:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_Backwards"));
        GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
        {
            GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_Backwards"));
        }), 3.85f, false);
        break;
			
    default:
        break;
    }
}

void AOPDiavolo::PlayDiavoloRandomDamagedMontage()
{
    if (GetChampionAnimInstance() == nullptr) return;
    int32 Section = FMath::RandRange(0, 4);
    FTimerHandle DeadTimer;
    GetChampionAnimInstance()->Montage_Play(DamagedAnimMontage);
    switch (Section)
    {								
    case 0:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_2"));
        GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
        {
            GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_2"));
        }), 4.45f, false);
        break;
				
    case 1:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_3"));
        GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
        {
            GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_3"));
        }), 3.5f, false);
        break;
				
    case 2:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_4"));
        GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
        {
            GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_4"));
        }), 3.4f, false);
        break;

    case 3:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_Backwards"));
        GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
        {
            GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_Backwards"));
        }), 3.85f, false);
        break;
			
    default:
        break;
    }
}
