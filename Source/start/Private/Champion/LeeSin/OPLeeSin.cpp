// Fill out your copyright notice in the Description page of Project Settings.

#include "Champion/LeeSin/OPLeeSin.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/LeeSin/OPLeeSinSonicWave.h"
#include "Champion/LeeSin/OPLeeSinDragonsRage.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

AOPLeeSin::AOPLeeSin()
{
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void AOPLeeSin::BeginPlay()
{
    Super::BeginPlay();
}

void AOPLeeSin::Passive()
{
    Super::Passive();
}

void AOPLeeSin::MeleeAttack()
{
    Super::MeleeAttack();

    if (!GetbMeleeAttack()) return;
    if (!GetOPPlayerController()) return;

    GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

    if (MouseCursorHit.bBlockingHit)
    {
        TurnCharacterToCursor(MouseCursorHit);
    }

    check(GetChampionAnimInstance());
    check(GetMeleeAttackAnimMontage());

    FTimerHandle Timer;
    GetWorldTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]
        {
            MeleeAttackTrace();
        }), 0.25f, false);

    switch (MeleeAttackComboCount)
    {
    case 0:
        GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
        GetChampionAnimInstance()->Montage_JumpToSection(FName("1"), MeleeAttackAnimMontage);
        GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
        MeleeAttackComboCount++;
        break;

    case 1:
        GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
        GetChampionAnimInstance()->Montage_JumpToSection(FName("2"), MeleeAttackAnimMontage);
        GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimer);
        GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
        MeleeAttackComboCount++;
        break;

    case 2:
        GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
        GetChampionAnimInstance()->Montage_JumpToSection(FName("3"), MeleeAttackAnimMontage);
        GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimer);
        GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
        MeleeAttackComboCount++;
        break;

    case 3:
        GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
        GetChampionAnimInstance()->Montage_JumpToSection(FName("4"), MeleeAttackAnimMontage);
        MeleeAttackComboCount = 0;
        break;

    default:
        ;
    }

    SetbMeleeAttack_False();
    SetbMeleeAttack_False();
    GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPLeeSin::SetbMeleeAttack_True, GetMeleeAttackCooltime(), false);
}

bool AOPLeeSin::MeleeAttackTrace()
{
    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f, 80.f,
        UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

    for (auto& HitActor : HitResults)
    {
        TestDiavolo = Cast<AOPDiavolo>(HitActor.GetActor());

        if (TestDiavolo)
        {
            TestDiavolo->SetbIsDamagedTrue();
            TestDiavolo->GetChampionAnimInstance()->Montage_Play(TestDiavolo->GetDiavolo_DamagedByLeeSinMeleeAttack_AnimMontage());
            TestDiavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * MeleeAttack_Impulse, true);
            if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
            {
                TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
            }

            return true;
        }
    }

    return false;
}

void AOPLeeSin::Skill_1()
{
    Super::Skill_1();

    if (!GetbSkill_1()) return;
    if (!GetOPPlayerController()) return;

    GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

    if (MouseCursorHit.bBlockingHit)
    {
        TurnCharacterToCursor(MouseCursorHit);
    }

    check(GetChampionAnimInstance());
    check(GetSkill_1_AnimMontage());

    if (Skill_1_Stack == 1)
    {
        UE_LOG(LogTemp, Log, TEXT("Skill_1_Moving"));
        GetWorldTimerManager().ClearTimer(Skill_1_StackTimer);
        Skill_1_Stack = 0;

        TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
        if (TestDiavolo && TestDiavolo->bTrueSightOn)
        {
            Skill_1_Stack = 0;
            GetChampionAnimInstance()->Montage_Play(GetSkill_1_AnimMontage(), 1.0f);
            GetChampionAnimInstance()->Montage_JumpToSection(FName("DashB"), GetSkill_1_AnimMontage());

            FVector TargetLocation = TestDiavolo->GetActorLocation();
            FVector Direction = TargetLocation - GetActorLocation();
            float Distance = Direction.Size();
            float LaunchSpeed = Distance * Skill_1_Velocity;
            FVector LaunchVelocity = Direction.GetSafeNormal() * LaunchSpeed;
            LaunchCharacter(LaunchVelocity, true, true);

            GetWorldTimerManager().SetTimer(ResonatingStrike_DiavoloMotionTimer, FTimerDelegate::CreateLambda([&]
            {
                TestDiavolo->GetChampionAnimInstance()->Montage_Play(TestDiavolo->GetDiavolo_DamagedByLeeSinResonatingStrike_AnimMontage());
            }), Distance / LaunchSpeed, false);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Skill_1_SonicWave"));
        GetChampionAnimInstance()->Montage_Play(GetSkill_1_AnimMontage(), 1.0f);
        GetChampionAnimInstance()->Montage_JumpToSection(FName("SonicWave"), GetSkill_1_AnimMontage());
        GetWorldTimerManager().SetTimer(SonicWaveSpawnTimer, this, &AOPLeeSin::Skill_1_SonicWave, 0.25f, false);
        GetWorldTimerManager().SetTimer(Skill_1_CooltimeTimer, this, &AOPLeeSin::SetbSkill_1_True, GetSkill_1_Cooltime(), false);
    }
}

void AOPLeeSin::Skill_1_SonicWave()
{
    if (SonicWaveClass == nullptr) return;

    FVector CurrentLocation = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector SpawnLocation = CurrentLocation + ForwardVector * 100.0f;

    SonicWave = GetWorld()->SpawnActor<AOPLeeSinSonicWave>(SonicWaveClass, SpawnLocation, GetActorRotation());
    SonicWave->SetOwner(this);
}

void AOPLeeSin::Skill_2()
{
    Super::Skill_2();

    if (!GetbSkill_2()) return;
    if (!GetOPPlayerController()) return;

    GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
    if (!MouseCursorHit.bBlockingHit) return;
    if (MouseCursorHit.bBlockingHit) // 만약 반응이 block이라면 그 Hit 방향으로 캐릭터를 돌림
    {
        TurnCharacterToCursor(MouseCursorHit);
    }
    AOPChampion* TargetChampion = Cast<AOPChampion>(MouseCursorHit.GetActor());
    if (TargetChampion && TargetChampion != this)
    {
        float DashSpeed = 2000.0f;
        float DashDistance = 600.0f;


        DashToTarget(TargetChampion, DashSpeed, DashDistance);

        GetWorldTimerManager().SetTimer(DashCompleteTimer, this, &AOPLeeSin::OnDashCompleted, 0.1f, false);

        SetbSkill_2_False();
        GetWorldTimerManager().SetTimer(Skill_2_CooltimeTimer, this, &AOPLeeSin::SetbSkill_2_True, GetSkill_2_Cooltime(), false);
    }
}

void AOPLeeSin::OnDashCompleted()
{
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors, AOPChampion::StaticClass());

    for (AActor* Actor : OverlappingActors)
    {
        AOPChampion* OverlappingChampion = Cast<AOPChampion>(Actor);
        if (OverlappingChampion && OverlappingChampion != this)
        {
            ApplyShieldToAlly(OverlappingChampion);
        }
    }
}

void AOPLeeSin::ApplyShieldToAlly(AOPChampion* TargetChampion)
{
    if (!TargetChampion) return;

    TargetChampion->AddShield(ShieldAmountValue);

    if (ChampionAnimInstance && Passive_AnimMontage)
    {
        ChampionAnimInstance->Montage_Play(Passive_AnimMontage, 1.f);
        ChampionAnimInstance->Montage_JumpToSection(FName("Shield"), Passive_AnimMontage);
    }
}

void AOPLeeSin::Skill_3()
{
    Super::Skill_3();

    if (!GetbSkill_3()) return;

    GetWorldTimerManager().SetTimer(Skill_3_CastTimer, this, &AOPLeeSin::Skill_3_GroundSlam, 0.25f, false);
    
    SetbSkill_3_False();
    GetWorldTimerManager().SetTimer(Skill_3_CooltimeTimer, this, &AOPLeeSin::SetbSkill_3_True, GetSkill_3_Cooltime(), false);

    if (ChampionAnimInstance && Skill_3_AnimMontage)
    {
        ChampionAnimInstance->Montage_Play(Skill_3_AnimMontage, 1.f);
        ChampionAnimInstance->Montage_JumpToSection(FName("GroundSlam"), Skill_3_AnimMontage);
    }
    
    // 땅을 내려치는 동작이라 커서의 Hit 값은 없어도 될 거 같아요!!
    // if (!GetOPPlayerController()) return;
    //
    // GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
    //
    // if (MouseCursorHit.bBlockingHit)
    // {
    //     TurnCharacterToCursor(MouseCursorHit);
    //     Skill_3_GroundSlam();
    //
    //     SetbSkill_3_False();
    //     GetWorldTimerManager().SetTimer(Skill_3_CooltimeTimer, this, &AOPLeeSin::SetbSkill_3_True, GetSkill_3_Cooltime(), false);
    // }
}

void AOPLeeSin::Skill_3_GroundSlam()
{
    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    float EffectRadius = Skill_3_radious;
    float SlowAmount = Skill_3_slowAmount;
    float SlowDuration = Skill_3_slowDuration;

    UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * EffectRadius, EffectRadius,
        UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

    for (auto& HitActor : HitResults)
    {
        if (TestDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
        {
            TestDiavolo->SetbIsDamagedTrue();
            TestDiavolo->GetChampionAnimInstance()->Montage_Play(TestDiavolo->GetDiavolo_DamagedByLeeSinSkill_3_AnimMontage());
            TestDiavolo->ApplySlowEffect(SlowAmount, SlowDuration);
        }
    }
    // Skill_3_ApplySlowEffect();
}

void AOPLeeSin::Skill_3_ApplySlowEffect()
{
    // 수정 해야함
    // TArray<FHitResult> HitResults;
    // TArray<AActor*> ActorsToIgnore;
    // ActorsToIgnore.Add(this);
    //
    // float EffectRadius = Skill_3_radious;
    // float SlowAmount = Skill_3_slowAmount;
    // float SlowDuration = Skill_3_slowDuration;
    //
    // UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * EffectRadius, EffectRadius,
    //     UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);
    //
    // for (auto& HitActor : HitResults)
    // {
    //     if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
    //     {
    //         if (HitDiavolo)
    //         {
    //             HitDiavolo->SetbIsDamagedTrue();
    //             PlayDiavoloRandomDeadMontage();
    //             HitDiavolo->ApplySlowEffect(SlowAmount, SlowDuration);
    //         }
    //     }
    // }
}

void AOPLeeSin::Ult()
{
    Super::Ult();

    if (!GetbUlt()) return;
    if (!GetOPPlayerController()) return;

    GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

    if (MouseCursorHit.bBlockingHit)
    {
        TurnCharacterToCursor(MouseCursorHit);
    }

    check(GetChampionAnimInstance());
    check(GetSkill_2_AnimMontage());

    GetChampionAnimInstance()->Montage_Play(GetUlt_AnimMontage(), 1.0f);

    GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
    UE_LOG(LogTemp, Log, TEXT("Skill_Ult_DragonsRage"));
    GetWorldTimerManager().SetTimer(DragonsRageSpawnTimer, FTimerDelegate::CreateLambda([&]
    {
        UltTrace();
    }), 0.25f, false);
    GetWorldTimerManager().SetTimer(Ult_CooltimeTimer, this, &AOPLeeSin::SetbUlt_True, GetUlt_Cooltime(), false);
}

bool AOPLeeSin::UltTrace()
{
    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 450.f, 40.f,
        UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

    for (auto& HitActor : HitResults)
    {
        TestDiavolo = Cast<AOPDiavolo>(HitActor.GetActor());

        if (TestDiavolo)
        {
            FVector ImpactDirection = (TestDiavolo->GetActorLocation() - HitActor.ImpactPoint).GetSafeNormal();
            ImpactDirection.Z += Ult_Angle;
            ImpactDirection = ImpactDirection.GetSafeNormal();

            UE_LOG(LogTemp, Log, TEXT("Impact Direction: %s"), *ImpactDirection.ToString());

            TestDiavolo->SetbIsDamagedTrue();
            TestDiavolo->GetChampionAnimInstance()->Montage_Play(TestDiavolo->GetDiavolo_DamagedByLeeSinDragonsRage_AnimMontage());
            TestDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * Ult_Impulse, true);
            TestDiavolo->TurnCharacterToLocation(GetActorLocation());
            if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
            {
                TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
            }

            return true;
        }
    }
    return false;
}

void AOPLeeSin::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AOPLeeSin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AOPLeeSin::CreateMarkerOnTarget(AOPDiavolo* Target)
{
    if (MarkerMesh && Target)
    {
        MarkerMesh->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        MarkerMesh->SetRelativeLocation(FVector(0, 0, 100));
        MarkerMesh->SetVisibility(true);
    }
}

void AOPLeeSin::RemoveMarkerOnTarget(AOPDiavolo* Target)
{
    if (MarkerMesh && Target)
    {
        MarkerMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
        MarkerMesh->SetVisibility(false);
    }
}

void AOPLeeSin::AddShield(float ShieldAmount)
{
    ShieldAmountValue = ShieldAmount; // 멤버 변수에 값을 할당

    UE_LOG(LogTemp, Log, TEXT("Shield added: %f"), ShieldAmountValue);

    if (ShieldEffectComponent && ShieldEffect)
    {
        ShieldEffectComponent->Activate(true);
    }

    GetWorld()->GetTimerManager().SetTimer(FuzeTimerHandle, this, &AOPChampion::DeactivateShieldEffect, 3.0f, false);
}

void AOPLeeSin::DashToTarget(AOPChampion* TargetChampion, float DashSpeed, float DashDistance)
{
    if (!TargetChampion) return;

    FVector TargetLocation = TargetChampion->GetActorLocation();
    FVector StartLocation = GetActorLocation();
    FVector DashDirection = (TargetLocation - StartLocation).GetSafeNormal();

    FVector DashVelocity = DashDirection * DashSpeed;
    LaunchCharacter(DashVelocity, true, true);
}

void AOPLeeSin::ResetMeleeAttackComboCount()
{
    MeleeAttackComboCount = 0;
}

