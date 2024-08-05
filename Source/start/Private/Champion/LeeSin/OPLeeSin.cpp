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

void AOPLeeSin::BasicAttack()
{
    Super::BasicAttack();

    if (!bBasicAttack) return;
    if (OPPlayerController == nullptr) return;

    OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

    if (!MouseCursorHit.bBlockingHit) return;
    TurnCharacterToCursor(MouseCursorHit);

    GetWorldTimerManager().SetTimer(BasicAttackTraceTimerHandle, FTimerDelegate::CreateLambda([&]
    {
        BasicAttackTrace();
    }), 0.25f, false);

    if (ChampionAnimInstance && BasicAttackAnimMontage)
    {
        switch (BasicAttackComboCount)
        {
        case 0:
            ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
            ChampionAnimInstance->Montage_JumpToSection(FName("1"), BasicAttackAnimMontage);
            GetWorldTimerManager().SetTimer(BasicAttackComboCountTimerHandle, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
            BasicAttackComboCount++;
            break;

        case 1:
            ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
            ChampionAnimInstance->Montage_JumpToSection(FName("2"), BasicAttackAnimMontage);
            GetWorldTimerManager().ClearTimer(BasicAttackComboCountTimerHandle);
            GetWorldTimerManager().SetTimer(BasicAttackComboCountTimerHandle, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
            BasicAttackComboCount++;
            break;

        case 2:
            ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
            ChampionAnimInstance->Montage_JumpToSection(FName("3"), BasicAttackAnimMontage);
            GetWorldTimerManager().ClearTimer(BasicAttackComboCountTimerHandle);
            GetWorldTimerManager().SetTimer(BasicAttackComboCountTimerHandle, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
            BasicAttackComboCount++;
            break;

        case 3:
            ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
            ChampionAnimInstance->Montage_JumpToSection(FName("4"), BasicAttackAnimMontage);
            BasicAttackComboCount = 0;
            break;

        default:
            ;
        }
    }
    
    SetbBasicAttack_False();
    GetWorldTimerManager().SetTimer(BasicAttackCooltimeTimerHandle, this, &AOPLeeSin::SetbBasicAttack_True, GetBasicAttackCooltime(), false);
    StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPLeeSin::ResetChampionMovement, 0.7f, false);
}

bool AOPLeeSin::BasicAttackTrace()
{
    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f, 80.f,
        UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

    for (auto& HitActor : HitResults)
    {
        HitActor.Component->AddImpulse(BasicAttack_Impulse * GetActorForwardVector());
        if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
        {
            Diavolo->SetbIsDamagedTrue();
            Diavolo->GetChampionAnimInstance()->Montage_Play(Diavolo->GetDiavolo_DamagedByLeeSinMeleeAttack_AnimMontage());
            Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * BasicAttack_Impulse, true);
            if (!Diavolo->GetbCanBeTestedMultipleTimes())
            {
                Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
            }

            return true;
        }
    }

    return false;
}

void AOPLeeSin::Q()
{
    Super::Q();

    if (!bQ) return;
    if (OPPlayerController == nullptr) return;

    if (bCanResonate)
    {
        GetWorldTimerManager().ClearTimer(Skill_1_StackTimer);
        if (TestDiavolo && TestDiavolo->GetbTrueSightOn())
        {
            SetbIsResonating_True();
            TurnCharacterToLocation(TestDiavolo->GetActorLocation());
            StopChampionMovement();
            UE_LOG(LogTemp, Log, TEXT("Skill_1_CanResonate"));
            FVector TargetLocation = TestDiavolo->GetActorLocation();
            FVector Direction = TargetLocation - GetActorLocation();
            float Distance = Direction.Size();
            float LaunchSpeed = Distance * Skill_1_Velocity;
            FVector LaunchVelocity = Direction.GetSafeNormal() * Skill_1_ResonateSpeed;
            ProjectileMovementComponent->Velocity = LaunchVelocity;

            GetWorldTimerManager().SetTimer(ResonatingStrike_DiavoloMotionTimer, FTimerDelegate::CreateLambda([&]
            {
                ProjectileMovementComponent->Velocity = FVector::Zero();
                TestDiavolo->GetChampionAnimInstance()->Montage_Play(TestDiavolo->GetDiavolo_DamagedByLeeSinResonatingStrike_AnimMontage());
                SetbIsResonating_False();
                ResetChampionMovement();
            }), Distance / Skill_1_ResonateSpeed, false);
        }
    }
    else
    {
        OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
        
        if (!MouseCursorHit.bBlockingHit) return;
        TurnCharacterToCursor(MouseCursorHit);
        
        UE_LOG(LogTemp, Log, TEXT("Skill_1_SonicWave"));
        ChampionAnimInstance->Montage_Play(GetQ_AnimMontage(), 1.0f);
        ChampionAnimInstance->Montage_JumpToSection(FName("SonicWave"), GetQ_AnimMontage());
        GetWorldTimerManager().SetTimer(SonicWaveSpawnTimer, this, &AOPLeeSin::Skill_1_SonicWave, 0.25f, false);
        GetWorldTimerManager().SetTimer(Q_CooldownTimerHandle, this, &AOPLeeSin::SetbQ_True, GetQ_Cooldown(), false);

        StopChampionMovement();
        GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPLeeSin::ResetChampionMovement, 0.9f, false);
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

void AOPLeeSin::W()
{
    Super::W();

    if (!GetbW()) return;
    if (!OPPlayerController) return;

    OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
    if (!MouseCursorHit.bBlockingHit) return;
    if (MouseCursorHit.bBlockingHit) // ���� ������ block�̶�� �� Hit �������� ĳ���͸� ����
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

        SetbW_False();
        GetWorldTimerManager().SetTimer(W_CooldownTimerHandle, this, &AOPLeeSin::SetbW_True, GetW_Cooldown(), false);
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

    if (ChampionAnimInstance && Passive_AnimMontage)
    {
        ChampionAnimInstance->Montage_Play(Passive_AnimMontage, 1.f);
        ChampionAnimInstance->Montage_JumpToSection(FName("Shield"), Passive_AnimMontage);
    }
}

void AOPLeeSin::E()
{
    Super::E();

    if (!GetbE()) return;

    GetWorldTimerManager().SetTimer(Skill_3_CastTimer, this, &AOPLeeSin::Skill_3_GroundSlam, 0.25f, false);
    
    SetbE_False();
    GetWorldTimerManager().SetTimer(E_CooldownTimerHandle, this, &AOPLeeSin::SetbE_True, GetE_Cooldown(), false);

    StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPLeeSin::ResetChampionMovement, 0.9f, false);

    if (ChampionAnimInstance && E_AnimMontage)
    {
        ChampionAnimInstance->Montage_Play(E_AnimMontage, 1.f);
        ChampionAnimInstance->Montage_JumpToSection(FName("GroundSlam"), E_AnimMontage);
    }
    
    // ���� ����ġ�� �����̶� Ŀ���� Hit ���� ��� �� �� ���ƿ�!!
    // if (!OPPlayerController) return;
    //
    // OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
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
        if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
        {
            Diavolo->SetbIsDamagedTrue();
            Diavolo->SetbIsDeadTrue();
            Diavolo->GetChampionAnimInstance()->Montage_Play(Diavolo->GetDiavolo_DamagedByLeeSinSkill_3_AnimMontage());
            Diavolo->ApplySlowEffect(SlowAmount, SlowDuration);
        }
    }
    // Skill_3_ApplySlowEffect();
}

void AOPLeeSin::Skill_3_ApplySlowEffect()
{
    // ���� �ؾ���
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

void AOPLeeSin::R()
{
    Super::R();

    if (!bR) return;
    if (!OPPlayerController) return;

    OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

    if (!MouseCursorHit.bBlockingHit) return;
    TurnCharacterToCursor(MouseCursorHit);

    GetWorldTimerManager().SetTimer(DragonsRageSpawnTimer, FTimerDelegate::CreateLambda([&]
    {
        UltTrace();
    }), 0.25f, false);
    
    if (ChampionAnimInstance && W_AnimMontage)
    {
        ChampionAnimInstance->Montage_Play(GetR_AnimMontage(), 1.0f);
    }

    StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPLeeSin::ResetChampionMovement, 0.8f, false);

    SetbR_False();
    GetWorldTimerManager().SetTimer(R_CooldownTimerHandle, this, &AOPLeeSin::SetbR_True, GetR_Cooldown(), false);
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
        HitActor.Component->AddImpulse(Ult_Impulse * GetActorForwardVector());
        if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
        {
            FVector ImpactDirection = (Diavolo->GetActorLocation() - HitActor.ImpactPoint).GetSafeNormal();
            ImpactDirection.Z += Ult_Angle;
            ImpactDirection = ImpactDirection.GetSafeNormal();

            UE_LOG(LogTemp, Log, TEXT("Impact Direction: %s"), *ImpactDirection.ToString());

            Diavolo->SetbIsDamagedTrue();
            Diavolo->SetbIsDeadTrue();
            Diavolo->GetChampionAnimInstance()->Montage_Play(Diavolo->GetDiavolo_DamagedByLeeSinDragonsRage_AnimMontage());
            Diavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * Ult_Impulse, true);
            Diavolo->TurnCharacterToLocation(GetActorLocation());
            if (!Diavolo->GetbCanBeTestedMultipleTimes())
            {
                Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
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
    ShieldAmountValue = ShieldAmount; // ��� ������ ���� �Ҵ�

    UE_LOG(LogTemp, Log, TEXT("Shield added: %f"), ShieldAmountValue);
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
    BasicAttackComboCount = 0;
}
