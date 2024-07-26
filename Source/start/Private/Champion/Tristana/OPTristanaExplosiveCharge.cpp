// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Tristana/OPTristanaExplosiveCharge.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Champion/OPChampion.h"
#include "Components/CapsuleComponent.h"
#include "Champion/Tristana/OPTristana.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AOPTristanaExplosiveCharge::BeginPlay()
{
    Super::BeginPlay();

    // Set a timer to automatically explode after a certain time
    GetWorld()->GetTimerManager().SetTimer(ExplosiveCharge_ExplosionTimerHandle, this, &AOPTristanaExplosiveCharge::Explode, ExplosiveCharge_TimeToExplode, false);
}

void AOPTristanaExplosiveCharge::InitializeCharge(AActor* TargetActor, float Damage, float ExplosionRadius, float TimeToExplode)
{
    if (Cast<AOPDiavolo>(TargetActor)) 
    {
        ExplosiveCharge_Target = TargetActor;
        ExplosiveCharge_Damage = Damage;
        ExplosiveCharge_ExplosionRadius = ExplosionRadius;
        ExplosiveCharge_TimeToExplode = TimeToExplode;

        // Attach to the target actor
        if (ExplosiveCharge_Target)
        {
            AttachToActor(ExplosiveCharge_Target, FAttachmentTransformRules::KeepRelativeTransform);
        }
    }
    
}

void AOPTristanaExplosiveCharge::OnHit()
{
    ExplosiveCharge_HitCount++;
    if (ExplosiveCharge_HitCount >= ExplosiveCharge_RequiredHits)
    {
        ExplosiveCharge_HitCount = 0;
        Explode();
    }
}

void AOPTristanaExplosiveCharge::Explode()
{
    // Apply radial damage
    //UGameplayStatics::ApplyRadialDamage(this, Skill_3_Damage, GetActorLocation(), Skill_3_ExplosionRadius, nullptr, TArray<AActor*>(), this, GetInstigatorController(), true);
    //데미지 입히기 대신 디아볼로가 쓰러지는 모션 삽입
    // Optionally, spawn particle effects or play sound here

    // Destroy the explosive charge actor
    Destroy();
}


