// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Zac/OPZac.h"

#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Player/OPPlayerController.h"

AOPZac::AOPZac()
{
	PhysicsHandle_RightHand = CreateDefaultSubobject<UPhysicsHandleComponent>("PhysicsHandle_RightHand");
	PhysicsHandle_LeftHand = CreateDefaultSubobject<UPhysicsHandleComponent>("PhysicsHandle_LeftHand");

	Q_GrabPoint_RightHand = CreateDefaultSubobject<USceneComponent>("Q_GrabPoint_Right");
	Q_GrabPoint_RightHand->SetupAttachment(GetMesh(), "r_hand");

	Q_GrabPoint_LeftHand = CreateDefaultSubobject<USceneComponent>("Q_GrabPoint_Left");
	Q_GrabPoint_LeftHand->SetupAttachment(GetMesh(), "l_hand");
}

void AOPZac::BeginPlay()
{
	Super::BeginPlay();
}

void AOPZac::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bQ_IsStretching_LeftHand)
	{
		Q_Trace_Catch_First();
	}
	if (bQ_IsStretching_RightHand)
	{
		Q_Trace_Catch_Second();
	}
	if (bQ_CaughtTarget_LeftHand)
	{
		PhysicsHandle_LeftHand->SetTargetLocationAndRotation(Q_GrabPoint_LeftHand->GetComponentLocation(), GetActorRotation());
	}
	if (bQ_CaughtTarget_RightHand)
	{
		PhysicsHandle_RightHand->SetTargetLocationAndRotation(Q_GrabPoint_RightHand->GetComponentLocation(), GetActorRotation());
	}
	if (bE_IsInAir)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			E_OnLanding();
			bE_IsInAir = false;
		}
	}
}

void AOPZac::BasicAttack()
{
	Super::BasicAttack();
	
	if (!bBasicAttack) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage);
		int32 RandomValue = FMath::RandRange(0, 3);

		GetWorldTimerManager().SetTimer(BasicAttack_Trace_TimerHandle, this, &AOPZac::BasicAttack_Trace, 0.25f, false);
		switch (RandomValue)
		{
		case 0:
			ChampionAnimInstance->Montage_JumpToSection("1", BasicAttack_AnimMontage);
			break;
		case 1:
			ChampionAnimInstance->Montage_JumpToSection("2", BasicAttack_AnimMontage);
			break;
		case 2:
			ChampionAnimInstance->Montage_JumpToSection("3", BasicAttack_AnimMontage);
			break;
		case 3:
			ChampionAnimInstance->Montage_JumpToSection("4", BasicAttack_AnimMontage);
			break;
		default:
			break;
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPZac::ResetChampionMovement, 1.05f, false);
	bBasicAttack = false;
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPZac::SetbBasicAttack_True, BasicAttack_Cooldown, false);
}

void AOPZac::BasicAttack_Trace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitResult.GetActor()))
	{
		Diavolo->SetbIsDamagedTrue();
		Diavolo->PlayDiavoloRandomDeadMontage();
		Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * BasicAttack_Strength, true);
		if (!Diavolo->GetbCanBeTestedMultipleTimes())
		{
			Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
	}
}

void AOPZac::Q()
{
	Super::Q();

	if (!bQ) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	if (!bQ_CaughtTarget_LeftHand)
	{
		GetWorldTimerManager().SetTimer(Q_StretchCast_LeftHand_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			bQ_IsStretching_LeftHand = true;
		}), 0.25f, false);

		GetWorldTimerManager().SetTimer(Q_StretchEnd_LeftHand_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			bQ_IsStretching_LeftHand = false;
		}), 0.63f, false);

		if (ChampionAnimInstance && Q_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(Q_AnimMontage);
			ChampionAnimInstance->Montage_JumpToSection("Q_LeftHand", Q_AnimMontage);
		}
		StopChampionMovement();
		GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPZac::ResetChampionMovement, 1.f, false);
	}
	else if (bQ_CaughtTarget_LeftHand)
	{
		GetWorldTimerManager().SetTimer(Q_StretchCast_RightHand_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			bQ_IsStretching_RightHand = true;
		}), 0.25f, false);

		GetWorldTimerManager().SetTimer(Q_StretchEnd_RightHand_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			bQ_IsStretching_RightHand = false;
		}), 0.55f, false);

		if (ChampionAnimInstance && Q_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(Q_AnimMontage);
			ChampionAnimInstance->Montage_JumpToSection("Q_RightHand", Q_AnimMontage);
		}
		GetWorldTimerManager().SetTimer(Q_SlamCast_TimerHandle, this, &AOPZac::Q_Trace_Catch_Second, 0.25f, false);
		StopChampionMovement();
		GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPZac::ResetChampionMovement, 0.2f, false);
	}
	if (bQ_IsReadyToSlam)
	{
		GetWorldTimerManager().SetTimer(Q_SlamCast_TimerHandle, this, &AOPZac::Q_Slam, 0.55f, false);
		if (ChampionAnimInstance && Q_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(Q_AnimMontage);
			ChampionAnimInstance->Montage_JumpToSection("Q_Slam", Q_AnimMontage);
		}
		StopChampionMovement();
		GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPZac::ResetChampionMovement, 0.7f, false);
		bQ = false;
		GetWorldTimerManager().SetTimer(Q_Cooldown_TimerHandle, this, &AOPZac::SetbQ_True, Q_Cooldown, false);
	}
}

void AOPZac::Q_Trace_Catch_First()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Q_GrabPoint_LeftHand->GetComponentLocation(), Q_GrabPoint_LeftHand->GetComponentLocation(), Q_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	if (Q_CaughtTarget = Cast<AOPChampion>(HitResult.GetActor()))
	{
		bQ_IsStretching_LeftHand = false;
		bQ_CaughtTarget_LeftHand = true;
		UPrimitiveComponent* PrimitiveComponent = HitResult.GetComponent();
		if (Q_CaughtTarget->GetRootComponent()->IsSimulatingPhysics())
		{
			PhysicsHandle_LeftHand->GrabComponentAtLocationWithRotation(PrimitiveComponent, "Root", Q_CaughtTarget->GetActorLocation(), FRotator::ZeroRotator);
		}
	}
}

void AOPZac::Q_Trace_Catch_Second()
{
	if (Q_CaughtTarget == nullptr) return;
	if (bQ_CaughtTarget_LeftHand == false) return;
	
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Q_GrabPoint_RightHand->GetComponentLocation(), Q_GrabPoint_RightHand->GetComponentLocation(), Q_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	if (Q_SlamTarget = Cast<AOPChampion>(HitResult.GetActor()))
	{
		bQ_IsStretching_RightHand = false;
		bQ_CaughtTarget_RightHand = true;
		bQ_IsReadyToSlam = true;
		UPrimitiveComponent* PrimitiveComponent = HitResult.GetComponent();
		if (Q_SlamTarget->GetRootComponent()->IsSimulatingPhysics())
		{
			PhysicsHandle_RightHand->GrabComponentAtLocationWithRotation(PrimitiveComponent, "Root", Q_SlamTarget->GetActorLocation(), FRotator::ZeroRotator);
		}
	}
}

void AOPZac::Q_Slam()
{
	if (Q_CaughtTarget == nullptr) return;
	if (Q_SlamTarget == nullptr) return;

	bQ_IsReadyToSlam = false;
	bQ_CaughtTarget_RightHand = false;
	bQ_CaughtTarget_LeftHand = false;
	bQ_IsStretching_RightHand = false;
	bQ_IsStretching_LeftHand = false;
	
	PhysicsHandle_LeftHand->ReleaseComponent();
	PhysicsHandle_RightHand->ReleaseComponent();
	
	FVector CaughtTargetVector = (Q_SlamTarget->GetActorLocation() - Q_CaughtTarget->GetActorLocation()).GetSafeNormal();
	FVector SlamTargetVector = -CaughtTargetVector;
	Q_CaughtTarget->GetCapsuleComponent()->AddImpulse(CaughtTargetVector * Q_SlamStrength, NAME_None, true);
	Q_SlamTarget->GetCapsuleComponent()->AddImpulse(SlamTargetVector * Q_SlamStrength, NAME_None, true);
}

void AOPZac::W()
{
	Super::W();
}

void AOPZac::W_Trace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * E_Radius, E_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

	for (auto& HitActor : HitResults)
	{
	}
}

void AOPZac::E()
{
	Super::E();

	if (!bE) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	if (!bE_CanJump)
	{
		bE_CanJump = true;
		ChampionAnimInstance->Montage_Play(E_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection("E_Charge", E_AnimMontage);
	}
	else if (bE_CanJump)
	{
		bE_IsInAir = true;
		bE_CanJump = false;
		ChampionAnimInstance->Montage_Stop(0.1f, E_AnimMontage);
		// ChampionAnimInstance->Montage_Play(E_AnimMontage);
		// ChampionAnimInstance->Montage_JumpToSection("E_Jump", E_AnimMontage);
		LaunchCharacter(GetActorForwardVector() * E_Speed_XY + GetActorUpVector() * E_Speed_Z, true, true);
	}
}

void AOPZac::E_OnLanding()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * E_Radius, E_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				FVector ImpulseDirection = (HitDiavolo->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				ImpulseDirection.Z = 0.f;
				ImpulseDirection.Z = E_Angle;
				HitDiavolo->LaunchCharacter(ImpulseDirection * E_Strength, true, true);
				
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
			}
		}
	}

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection("E_Land", E_AnimMontage);
	}
}

void AOPZac::R()
{
	Super::R();
	
	if (!bR) return;

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage);
	}
}
