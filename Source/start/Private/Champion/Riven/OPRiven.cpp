// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Riven/OPRiven.h"
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"

AOPRiven::AOPRiven()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");

	R_SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sword Mesh");
	R_SwordMesh->SetupAttachment(GetMesh(), FName(TEXT("R Sword Socket")));
	R_SwordMesh->SetHiddenInGame(true);
}

void AOPRiven::BeginPlay()
{
	Super::BeginPlay();
}

void AOPRiven::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AOPRiven::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(BasicAttack_Trace_TimerHandle, this, &AOPRiven::BasicAttack_Trace, 0.25f, false);

	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage);
		int32 RandomValue = FMath::RandRange(0, 2);

		if (bR_IsActivated)
		{
			switch (RandomValue)
			{
			case 0:
				ChampionAnimInstance->Montage_JumpToSection("BasicAttack_1R", BasicAttack_AnimMontage);
				break;
			case 1:
				ChampionAnimInstance->Montage_JumpToSection("BasicAttack_2R", BasicAttack_AnimMontage);
				break;
			case 2:
				ChampionAnimInstance->Montage_JumpToSection("BasicAttack_3", BasicAttack_AnimMontage);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (RandomValue)
			{
			case 0:
				ChampionAnimInstance->Montage_JumpToSection("BasicAttack_1", BasicAttack_AnimMontage);
				break;
			case 1:
				ChampionAnimInstance->Montage_JumpToSection("BasicAttack_2", BasicAttack_AnimMontage);
				break;
			case 2:
				ChampionAnimInstance->Montage_JumpToSection("BasicAttack_3", BasicAttack_AnimMontage);
				break;
			default:
				break;
			}
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPRiven::ResetChampionMovement, 1.05f, false);
	bBasicAttack = false;
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPRiven::SetbBasicAttack_True, BasicAttack_Cooldown, false);
}

void AOPRiven::BasicAttack_Trace()
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

void AOPRiven::Q()
{
	Super::Q();

	if (!bQ) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	if (ChampionAnimInstance && Q_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Q_AnimMontage);

		switch (Q_Step)
		{
		case 1:
			ChampionAnimInstance->Montage_JumpToSection(FName("Q_1"), Q_AnimMontage);
			LaunchCharacter(GetActorForwardVector() * Q_Speed_XY, true, true);
			GetWorldTimerManager().SetTimer(Q_Trace_TimerHandle, this, &AOPRiven::Q_Trace, false, 1.f);
			Q_Step++;
			break;
		case 2:
			ChampionAnimInstance->Montage_JumpToSection(FName("Q_2"), Q_AnimMontage);
			LaunchCharacter(GetActorForwardVector() * Q_Speed_XY, true, true);
			GetWorldTimerManager().SetTimer(Q_Trace_TimerHandle, this, &AOPRiven::Q_Trace, false, 1.f);
			Q_Step++;
			break;
		case 3:
			ChampionAnimInstance->Montage_JumpToSection(FName("Q_3"), Q_AnimMontage);
			LaunchCharacter(GetActorForwardVector() * Q_Speed_XY + GetActorUpVector() * Q_Speed_Z, true, true);
			GetWorldTimerManager().SetTimer(Q_Trace_TimerHandle, this, &AOPRiven::Q_Trace_Third, false, 1.f);
			Q_Step = 1;
			SetbQ_False();
			GetWorldTimerManager().SetTimer(Q_Cooldown_TimerHandle, this, &AOPRiven::SetbQ_True, Q_Cooldown, false);
			break;
		default:
			break;
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPRiven::ResetChampionMovement, 0.4f, false);
}

void AOPRiven::Q_Trace() const
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * Q_Range, Q_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * Q_Strength, true);
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
}

void AOPRiven::Q_Trace_Third() const
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * Q_Range, Q_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			Diavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(), Q_Width, Q_Strength, RIF_Linear, true);;
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
}

void AOPRiven::W()
{
	Super::W();

	if (!bW) return;
	
	W_Trace();
	
	if (ChampionAnimInstance && W_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(W_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPRiven::ResetChampionMovement, 0.3f, false);
	SetbW_False();
	GetWorldTimerManager().SetTimer(W_Cooldown_TimerHandle, this, &AOPRiven::SetbW_True, W_Cooldown, false);
}

void AOPRiven::W_Trace() const
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), W_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			Diavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(), W_Width, W_Strength, RIF_Linear, true);;
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
}

void AOPRiven::E()
{
	Super::E();

	if (!bE) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	LaunchCharacter(GetActorForwardVector() * E_Speed, true, true);

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPRiven::ResetChampionMovement, 0.1f, false);
	SetbE_False();
	GetWorldTimerManager().SetTimer(E_Cooldown_TimerHandle, this, &AOPRiven::SetbE_True, E_Cooldown, false);
}

void AOPRiven::R()
{
	Super::R();

	if (!bR) return;

	if (!bR_CanSlash)
	{
		bR_IsActivated = true;
		bR_CanSlash = true;
		GetWorldTimerManager().SetTimer(R_Reinforce_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			R_SwordMesh->SetHiddenInGame(false);
		}), 0.25f, false);
		GetWorldTimerManager().SetTimer(R_ReinforceEnd_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			bR_IsActivated = false;
			R_SwordMesh->SetHiddenInGame(true);
		}), 15.5f, false);
		if (ChampionAnimInstance && R_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(R_AnimMontage);
			ChampionAnimInstance->Montage_JumpToSection("R_Reinforce", R_AnimMontage);
		}
	}

	else if (bR_CanSlash)
	{
		bR_CanSlash = false;
		if (OPPlayerController == nullptr) return;
		OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

		if (!MouseCursorHit.bBlockingHit) return;
		TurnCharacterToCursor(MouseCursorHit);

		if (ChampionAnimInstance && R_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(R_AnimMontage);
			ChampionAnimInstance->Montage_JumpToSection("R_Slash", R_AnimMontage);
		}

		StopChampionMovement();
		GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPRiven::ResetChampionMovement, 1.05f, false);
		SetbR_False();
		GetWorldTimerManager().SetTimer(R_Cooldown_TimerHandle, this, &AOPRiven::SetbR_True, R_Cooldown, false);
	}
}
