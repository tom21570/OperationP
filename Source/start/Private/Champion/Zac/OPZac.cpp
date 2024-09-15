// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Zac/OPZac.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/Zac/OPZacChunk.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Player/OPPlayerController.h"

AOPZac::AOPZac()
{
	Passive_SpawnPoint = CreateDefaultSubobject<USceneComponent>("Passive_SpawnPoint");
	Passive_SpawnPoint->SetupAttachment(GetRootComponent());
	
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

	Zac_OriginalSizeIndex = GetCapsuleComponent()->GetComponentScale();
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
	if (bR_IsInAir)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			R_OnLanding();
			bR_IsInAir = false;
		}
	}
}

void AOPZac::Passive()
{
	Super::Passive();

	FRotator SpawnRotation = Passive_SpawnPoint->GetComponentRotation();
	SpawnRotation.Yaw = FMath::RandRange(0, 90);
	Passive_ChunkStorage = GetWorld()->SpawnActor<AOPZacChunk>(Passive_ChunkClass, Passive_SpawnPoint->GetComponentLocation(), SpawnRotation);
	Passive_ChunkStorage->SetOwner(this);
}

void AOPZac::ChangeZacSize()
{
	if (bZac_IsBig)
	{
		bZac_IsBig = false;
		GetCapsuleComponent()->SetWorldScale3D(Zac_OriginalSizeIndex);
	}

	else
	{
		bZac_IsBig = true;
		GetCapsuleComponent()->SetWorldScale3D(Zac_OriginalSizeIndex * Zac_IncreasedSizeIndex);
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

	if (bZac_IsBig)
	{
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * BasicAttack_Range_Big, BasicAttack_Width_Big,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitResult.GetActor()))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * BasicAttack_Strength_Big, true);
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
	else
	{
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
	const TArray<AActor*> ActorsToIgnore;

	if (bZac_IsBig)
	{
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Q_GrabPoint_LeftHand->GetComponentLocation(), Q_GrabPoint_LeftHand->GetComponentLocation(), Q_Width_Big,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

		if (Q_CaughtTarget = Cast<AOPChampion>(HitResult.GetActor()))
		{
			Passive();
			bQ_IsStretching_LeftHand = false;
			bQ_CaughtTarget_LeftHand = true;
			UPrimitiveComponent* PrimitiveComponent = HitResult.GetComponent();
			if (Q_CaughtTarget->GetRootComponent()->IsSimulatingPhysics())
			{
				PhysicsHandle_LeftHand->GrabComponentAtLocationWithRotation(PrimitiveComponent, "Root", Q_CaughtTarget->GetActorLocation(), FRotator::ZeroRotator);
			}
		}
	}
	else
	{
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Q_GrabPoint_LeftHand->GetComponentLocation(), Q_GrabPoint_LeftHand->GetComponentLocation(), Q_Width,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

		if (Q_CaughtTarget = Cast<AOPChampion>(HitResult.GetActor()))
		{
			Passive();
			bQ_IsStretching_LeftHand = false;
			bQ_CaughtTarget_LeftHand = true;
			UPrimitiveComponent* PrimitiveComponent = HitResult.GetComponent();
			if (Q_CaughtTarget->GetRootComponent()->IsSimulatingPhysics())
			{
				PhysicsHandle_LeftHand->GrabComponentAtLocationWithRotation(PrimitiveComponent, "Root", Q_CaughtTarget->GetActorLocation(), FRotator::ZeroRotator);
			}
		}
	}
}

void AOPZac::Q_Trace_Catch_Second()
{
	if (Q_CaughtTarget == nullptr) return;
	if (bQ_CaughtTarget_LeftHand == false) return;
	
	FHitResult HitResult;
	const TArray<AActor*> ActorsToIgnore;

	if (bZac_IsBig)
	{
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Q_GrabPoint_RightHand->GetComponentLocation(),
			Q_GrabPoint_RightHand->GetComponentLocation(), Q_Width_Big,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

		if (Q_SlamTarget = Cast<AOPChampion>(HitResult.GetActor()))
		{
			Passive();
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
	else
	{
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Q_GrabPoint_RightHand->GetComponentLocation(),
			Q_GrabPoint_RightHand->GetComponentLocation(), Q_Width,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

		if (Q_SlamTarget = Cast<AOPChampion>(HitResult.GetActor()))
		{
			Passive();
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
	
	const FVector CaughtTargetVector = (Q_SlamTarget->GetActorLocation() - Q_CaughtTarget->GetActorLocation()).GetSafeNormal();
	const FVector SlamTargetVector = -CaughtTargetVector;
	Q_CaughtTarget->GetCapsuleComponent()->AddImpulse(CaughtTargetVector * Q_SlamStrength, NAME_None, true);
	Q_SlamTarget->GetCapsuleComponent()->AddImpulse(SlamTargetVector * Q_SlamStrength, NAME_None, true);

	if (Zac_Splash_NiagaraSystem_Big)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Zac_Splash_NiagaraSystem_Big, GetActorLocation() + GetActorForwardVector() * 70.f);
	}

	else
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Zac_Splash_NiagaraSystem, GetActorLocation() + GetActorForwardVector() * 70.f);
	}
	Passive();
}

void AOPZac::W()
{
	Super::W();

	if (!bW) return;

	W_Trace();

	bW = false;
	GetWorldTimerManager().SetTimer(W_Cooldown_TimerHandle, this, &AOPZac::SetbW_True, W_Cooldown, false);
}

void AOPZac::W_Trace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	if (bZac_IsBig)
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), W_Radius_Big,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
			ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

		for (auto& HitActor : HitResults)
		{
			if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
			{
				if (HitDiavolo)
				{
					Passive();
					HitDiavolo->SetbIsDamagedTrue();
					HitDiavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(), W_Radius_Big,
						W_Strength_Big, ERadialImpulseFalloff::RIF_Linear, true);
					HitDiavolo->PlayDiavoloRandomDeadMontage();
				}
			}
		}

		if (Zac_Splash_NiagaraSystem_Big)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Zac_Splash_NiagaraSystem_Big, GetActorLocation());
		}
	}

	else
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), W_Radius,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

		for (auto& HitActor : HitResults)
		{
			if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
			{
				if (HitDiavolo)
				{
					Passive();
					HitDiavolo->SetbIsDamagedTrue();
					HitDiavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(),
						W_Radius, W_Strength, RIF_Linear, true);
					HitDiavolo->PlayDiavoloRandomDeadMontage();
				}
			}
		}

		if (Zac_Splash_NiagaraSystem)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Zac_Splash_NiagaraSystem, GetActorLocation());
		}
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
		
		if (bZac_IsBig)
		{
			LaunchCharacter(GetActorForwardVector() * E_Speed_XY_Big + GetActorUpVector() * E_Speed_Z_Big, true, true);
		}

		else
		{
			LaunchCharacter(GetActorForwardVector() * E_Speed_XY + GetActorUpVector() * E_Speed_Z, true, true);
		}
	}
}

void AOPZac::E_OnLanding()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	if (bZac_IsBig)
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), E_Radius_Big,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
			ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

		for (auto& HitActor : HitResults)
		{
			if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
			{
				if (HitDiavolo)
				{
					Passive();
					HitDiavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(),
						E_Radius_Big, E_Strength_Big, RIF_Linear, true);
					HitDiavolo->SetbIsDamagedTrue();
					HitDiavolo->PlayDiavoloRandomDeadMontage();
				}
			}
		}
		
		if (Zac_Splash_NiagaraSystem_Big)
		{
			FVector ZacLocation = GetActorLocation();
			ZacLocation.Z -= 78.f;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Zac_Splash_NiagaraSystem_Big, ZacLocation);
		}
	}

	else
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), E_Radius,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
			ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

		for (auto& HitActor : HitResults)
		{
			if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
			{
				if (HitDiavolo)
				{
					Passive();
					HitDiavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(),
						E_Radius, E_Strength, RIF_Linear, true);
					HitDiavolo->SetbIsDamagedTrue();
					HitDiavolo->PlayDiavoloRandomDeadMontage();
				}
			}
		}

		if (Zac_Splash_NiagaraSystem)
		{
			FVector ZacLocation = GetActorLocation();
			ZacLocation.Z -= 78.f;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Zac_Splash_NiagaraSystem, ZacLocation);
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

	if (bZac_IsBig)
	{
		LaunchCharacter(GetActorUpVector() * R_Speed_Z_Big, true, true);
	}

	else
	{
		LaunchCharacter(GetActorUpVector() * R_Speed_Z, true, true);
	}

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage);
	}
	
	GetWorldTimerManager().SetTimer(R_TraceLoop_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		bR_IsInAir = true;
		ChampionAnimInstance->Montage_Stop(0.f, R_AnimMontage);
	}), 0.5f, false);
}

void AOPZac::R_OnLanding()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	if (bZac_IsBig)
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), R_Radius_Big,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
			ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

		for (auto& HitActor : HitResults)
		{
			if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
			{
				if (HitDiavolo)
				{
					Passive();
					HitDiavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(),
						R_Radius_Big, R_Strength_Big, RIF_Linear, true);
					
					HitDiavolo->SetbIsDamagedTrue();
					HitDiavolo->PlayDiavoloRandomDeadMontage();
				}
			}
		}

		if (Zac_Splash_NiagaraSystem_Big)
		{
			FVector ZacLocation = GetActorLocation();
			ZacLocation.Z -= 78.f;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Zac_Splash_NiagaraSystem_Big, ZacLocation);
		}
	}

	else
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), R_Radius,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
			ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

		for (auto& HitActor : HitResults)
		{
			if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
			{
				if (HitDiavolo)
				{
					Passive();
					HitDiavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(),
						R_Radius, R_Strength, RIF_Linear, true);
					
					HitDiavolo->SetbIsDamagedTrue();
					HitDiavolo->PlayDiavoloRandomDeadMontage();
				}
			}
		}

		if (Zac_Splash_NiagaraSystem)
		{
			FVector ZacLocation = GetActorLocation();
			ZacLocation.Z -= 78.f;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Zac_Splash_NiagaraSystem, ZacLocation);
		}
	}

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection("E_Land", E_AnimMontage);
	}
}
