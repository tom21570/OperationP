// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

#include "Champion/LeeSin/OPLeeSinSonicWave.h"
#include "Champion/OPChampion.h"
#include "Animation/AnimInstance.h" // �ִϸ��̼� �ν��Ͻ� ��� ����
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Champion/LeeSin/OPLeeSin.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AOPLeeSinSonicWave::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    TraceStaticMesh();
    
    if (bStaticMeshTraceSucceeded)
    {
        TraceLeeSin();
    }
}

void AOPLeeSinSonicWave::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
    {
        AOPDiavolo* Diavolo = Cast<AOPDiavolo>(OtherActor);
        AOPLeeSin* LeeSinOwner = Cast<AOPLeeSin>(GetOwner());
        if (Diavolo)
        {
            LeeSinOwner->SetResonateTarget(Diavolo);
            LeeSinOwner->SetTestDiavolo(Diavolo);

            LeeSinOwner->SetbCanResonate_True();
            Diavolo->SetbTrueSightOn_True();

            Diavolo->GetChampionAnimInstance()->Montage_Play(Diavolo->GetDiavolo_DamagedByLeeSinSonicWave_AnimMontage());

            if (!Diavolo->GetbCanBeTestedMultipleTimes())
            {
                Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
            }
            if (LeeSinOwner)
            {
                LeeSinOwner->GetWorldTimerManager().SetTimer(LeeSinOwner->Skill_1_StackTimer, [LeeSinOwner, Diavolo]()
                {
                    LeeSinOwner->SetbQ_True();
                    LeeSinOwner->RemoveMarkerOnTarget(Diavolo);
                }, LeeSinOwner->Skill_1_StackTime, false); // ���� ����

                LeeSinOwner->CreateMarkerOnTarget(Diavolo);
            }
            Destroy();
        }
    }

    if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(OtherComp))
    {
        OPProjectileMovementComponent->Velocity = FVector::ZeroVector;
        GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, FString(TEXT("Block")));;
    }
}

void AOPLeeSinSonicWave::TraceStaticMesh()
{
    if (GetOwner() == nullptr) return;
    
    FHitResult HitResult;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation(), 22.f,
        UEngineTypes::ConvertToTraceType(ECC_WorldStatic), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
    
    if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent()))
    {
        AOPLeeSin* LeeSinOwner = Cast<AOPLeeSin>(GetOwner());
        SetActorRotation(FRotator(-180.f, GetActorRotation().Yaw, GetActorRotation().Roll))     ;
        GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, FString(TEXT("Block")));
        OPProjectileMovementComponent->Velocity = (LeeSinOwner->GetActorLocation() - GetActorLocation());
        TraceLeeSin();

        bStaticMeshTraceSucceeded = true;
    }
}

void AOPLeeSinSonicWave::TraceLeeSin()
{
    if (GetOwner() == nullptr) return;
    
    FHitResult HitResult;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation(), 22.f,
        UEngineTypes::ConvertToTraceType(ECC_Pawn), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
    
    if (AOPLeeSin* LeeSin = Cast<AOPLeeSin>(HitResult.GetActor()))
    {
        Destroy();
    }
}
