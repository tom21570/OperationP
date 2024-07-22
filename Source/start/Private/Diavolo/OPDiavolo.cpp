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

void AOPDiavolo::PlayDeadMontage1()
{
    if (ChampionAnimInstance)
    {
        ChampionAnimInstance->Montage_JumpToSection(FName("Dead_1"));
    }
}

void AOPDiavolo::PlayDeadMontage2()
{
    if (ChampionAnimInstance)
    {
        ChampionAnimInstance->Montage_JumpToSection(FName("Dead_2"));
    }
}

void AOPDiavolo::PlayDeadMontage3()
{
    if (ChampionAnimInstance)
    {
        ChampionAnimInstance->Montage_JumpToSection(FName("Dead_3"));
    }
}

void AOPDiavolo::PlayDeadMontage4()
{
    if (ChampionAnimInstance)
    {
        ChampionAnimInstance->Montage_JumpToSection(FName("Dead_Backwards"));
    }
}

TObjectPtr<USkeletalMeshComponent> AOPDiavolo::GetDiavoloMesh() const
{
    return GetMesh();
}

void AOPDiavolo::PlayDiavoloRandomDeadMontage()
{
    if (GetChampionAnimInstance() == nullptr) return;
    int32 Section = FMath::RandRange(0, 4);
    GetChampionAnimInstance()->Montage_Play(DeadAnimMontage);
    switch (Section)
    {								
    case 0:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_1"));
        GetWorldTimerManager().SetTimer(DeadTimer1, this, &AOPDiavolo::PlayDeadMontage1, 4.45f, false);
        break;
				
    case 1:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_2"));
        GetWorldTimerManager().SetTimer(DeadTimer2, this, &AOPDiavolo::PlayDeadMontage2, 3.5f, false);
        break;
				
    case 2:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_3"));
        GetWorldTimerManager().SetTimer(DeadTimer3, this, &AOPDiavolo::PlayDeadMontage3, 3.4f, false);
        break;

    case 3:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_Backwards"));
        GetWorldTimerManager().SetTimer(DeadTimer4, this, &AOPDiavolo::PlayDeadMontage4, 3.85f, false);
        break;
			
    default:
        break;
    }
}

void AOPDiavolo::PlayDiavoloRandomDamagedMontage()
{
    if (GetChampionAnimInstance() == nullptr) return;
    int32 Section = FMath::RandRange(0, 2);
    FTimerHandle DeadTimer;
    GetChampionAnimInstance()->Montage_Play(DamagedAnimMontage);
    switch (Section)
    {								
    case 0:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Knockback"));
        break;
				
    case 1:
        GetChampionAnimInstance()->Montage_JumpToSection(FName("Hit"));
        break;
			
    default:
        break;
    }
}
