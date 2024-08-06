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
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AOPTristanaExplosiveCharge::BeginPlay()
{
    Super::BeginPlay();

    // Set a timer to automatically explode after a certain time
    GetWorld()->GetTimerManager().SetTimer(ExplosiveCharge_ExplosionTimerHandle, this, &AOPTristanaExplosiveCharge::Explode, E_TimeToExplode, false);

    GetWorldTimerManager().SetTimer(DecreaseVelocityTimerHandle, FTimerDelegate::CreateLambda([&]
    {
        GetOPProjectileMovementComponent()->HomingAccelerationMagnitude = 20000.f;
    }), 1.f, false);
}

void AOPTristanaExplosiveCharge::Explode()
{
    TArray<AActor*> OverlappingActors;
    RootCapsule->GetOverlappingActors(OverlappingActors);

    for (auto Actor : OverlappingActors)
    {
        UE_LOG(LogTemp, Warning,  TEXT("Explode"));
        if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(Actor))
        {
            FRotator ExplosionRotation = (Diavolo->GetActorLocation() - GetActorLocation()).Rotation();
            FRotator FinalRotation = FRotator(E_Angle, ExplosionRotation.Yaw, ExplosionRotation.Roll);
            Diavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(), E_ExplosionRadius, E_KnockbackStrength, RIF_Linear, true);
            Diavolo->GetCharacterMovement()->AddImpulse(FinalRotation.Vector() * E_KnockbackStrength, true);
        }
    }

    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation());
    Destroy();
}
