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

	if (bQ_IsStretching)
	{
		for (int32 i = 0; i < 30; i++)
		{
			Q_TickVariable++;
		}
		Q_Trace_Catch();
	}
	else
	{
		Q_TickVariable = 0;
	}
	if (bQ_CanSlam)
	{
		PhysicsHandle_LeftHand->SetTargetLocationAndRotation(Q_GrabPoint_LeftHand->GetComponentLocation(), GetActorRotation());
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

	if (!bQ_CanSlam)
	{
		GetWorldTimerManager().SetTimer(Q_StretchCast_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			bQ_IsStretching = true;
		}), 0.25f, false);

		GetWorldTimerManager().SetTimer(Q_StretchEnd_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			bQ_IsStretching = false;
		}), 0.55f, false);

		if (ChampionAnimInstance && Q_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(Q_AnimMontage);
			ChampionAnimInstance->Montage_JumpToSection("First", Q_AnimMontage);
		}
	}
	else
	{
		Q_TraceForSlamTarget();
		GetWorldTimerManager().SetTimer(Q_SlamCast_TimerHandle, this, &AOPZac::Q_Trace_Slam, 0.25f, false);
	}
}

void AOPZac::Q_Trace_Catch()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * Q_TickVariable, Q_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	if (Q_CaughtTarget = Cast<AOPChampion>(HitResult.GetActor()))
	{
		bQ_IsStretching = false;
		bQ_CanSlam = true;
		UPrimitiveComponent* PrimitiveComponent = HitResult.GetComponent();
		if (Q_CaughtTarget->GetRootComponent()->IsSimulatingPhysics())
		{
			PhysicsHandle_LeftHand->GrabComponentAtLocationWithRotation(PrimitiveComponent, "Root", Q_CaughtTarget->GetActorLocation(), FRotator::ZeroRotator);
		}
	}
}

void AOPZac::Q_TraceForSlamTarget()
{
	if (Q_CaughtTarget == nullptr) return;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f, Q_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	if (Q_SlamTarget = Cast<AOPChampion>(HitResult.GetActor()))
	{
		bQ_CanSlam = false;
		if (ChampionAnimInstance && BasicAttack_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage);
			ChampionAnimInstance->Montage_JumpToSection("Q_Punch", BasicAttack_AnimMontage);
		}
	}
}

void AOPZac::Q_Trace_Slam()
{
	if (Q_CaughtTarget == nullptr) return;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f, Q_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	if (Q_SlamTarget = Cast<AOPChampion>(HitResult.GetActor()))
	{
		UPrimitiveComponent* PrimitiveComponent = HitResult.GetComponent();
		PhysicsHandle_RightHand->GrabComponentAtLocationWithRotation(PrimitiveComponent, "Root", Q_SlamTarget->GetActorLocation(), FRotator::ZeroRotator);
		if (ChampionAnimInstance && Q_AnimMontage)
		{
			// ChampionAnimInstance->Montage_Play(Q_AnimMontage);
			// ChampionAnimInstance->Montage_JumpToSection("Second", Q_AnimMontage);
		}
		FVector CaughtTargetVector = (Q_SlamTarget->GetActorLocation() - Q_CaughtTarget->GetActorLocation()).GetSafeNormal();
		FVector SlamTargetVector = -CaughtTargetVector;
		Q_CaughtTarget->GetCharacterMovement()->AddImpulse(CaughtTargetVector * Q_SlamStrength, true);
		Q_SlamTarget->GetCharacterMovement()->AddImpulse(SlamTargetVector * Q_SlamStrength, true);
	}
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
}

void AOPZac::E_Trace()
{
}

void AOPZac::R()
{
	Super::R();
}
