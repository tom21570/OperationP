// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Galio/OPGalio.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/Galio/OPGalioWindblast.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"
#include "start/start.h"

AOPGalio::AOPGalio()
{
	Q_Windblast_SpawnPoint_1 = CreateDefaultSubobject<USceneComponent>("Q_Windblast_SpawnPoint_1");
	Q_Windblast_SpawnPoint_1->SetupAttachment(GetRootComponent());
	Q_Windblast_SpawnPoint_2 = CreateDefaultSubobject<USceneComponent>("Q_Windblast_SpawnPoint_2");
	Q_Windblast_SpawnPoint_2->SetupAttachment(GetRootComponent());

	W_NiagaraSystemComponent_Charging = CreateDefaultSubobject<UNiagaraComponent>("W_NiagaraSystemComponent_Charging");
	W_NiagaraSystemComponent_Charging->SetupAttachment(GetRootComponent());
}

void AOPGalio::BeginPlay()
{
	Super::BeginPlay();
}

void AOPGalio::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	W_Timeline.TickTimeline(DeltaSeconds);

	if (bE_IsDashing)
	{
		E_Trace();
	}

	if (bR_IsInAir)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			R_OnLanding();
		}
	}
}

void AOPGalio::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	// GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
	// {
	// 	BATrace();
	// }), 0.35f, false);

	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		if (bPassive)
		{
			bPassive = false;
			GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
			{
				BasicAttack_Trace_Passive();
			}), 0.35f, false);
			
			ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection("Passive", BasicAttack_AnimMontage);
		}

		else
		{
			switch (BasicAttack_ComboCount)
			{
			case 0:
				GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
				{
					BasicAttack_Trace_Up();
				}), 0.35f, false);
				
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("1"), BasicAttack_AnimMontage);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPGalio::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount++;
				break;

			case 1:
				GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
				{
					BasicAttack_Trace_Left();
				}), 0.35f, false);
				
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("2"), BasicAttack_AnimMontage);
				GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_TimerHandle);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPGalio::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount++;
				break;
				
			case 2:
				GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
				{
					BasicAttack_Trace_Right();
				}), 0.35f, false);
				
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("3"), BasicAttack_AnimMontage);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPGalio::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount++;
				break;

			case 3:
				GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
				{
					BasicAttack_Trace_Left();
				}), 0.35f, false);
				
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("4"), BasicAttack_AnimMontage);
				GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_TimerHandle);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPGalio::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount = 0;
				break;
				
			default:
				;
			}
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPGalio::ResetChampionMovement, 1.7f, false);
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPGalio::SetbBasicAttack_True, BasicAttack_Cooldown, false);
}

void AOPGalio::BasicAttack_Trace(const int32& Section) const
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			float Distance = FVector::Dist(GetActorLocation(), HitDiavolo->GetActorLocation());
			
			if (bPassive)
			{
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
				HitDiavolo->GetCharacterMovement()->AddImpulse(GetActorUpVector() * BasicAttack_Strength_Passive, true);
			}
			
			else
			{
				switch (Section)
				{
				case 0:
					HitDiavolo->GetCharacterMovement()->AddImpulse(GetActorUpVector() * BasicAttack_Strength / Distance, true);
					break;

				case 1:
					HitDiavolo->GetCharacterMovement()->AddImpulse(-GetActorRightVector() * BasicAttack_Strength / Distance, true);
					break;
					
				case 2:
					HitDiavolo->GetCharacterMovement()->AddImpulse(GetActorRightVector() * BasicAttack_Strength / Distance, true);
					break;
					
				case 3:
					HitDiavolo->GetCharacterMovement()->AddImpulse(-GetActorRightVector() * BasicAttack_Strength / Distance, true);
					break;

				default:
					break;
				}
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
			}
		}
	}
}

void AOPGalio::BasicAttack_Trace_Passive() const
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
				HitDiavolo->GetCharacterMovement()->AddImpulse(GetActorUpVector() * BasicAttack_Strength_Passive, true);
			}
		}
	}
}

void AOPGalio::BasicAttack_Trace_Up() const
{
TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			float Distance = FVector::Dist(GetActorLocation(), HitDiavolo->GetActorLocation());
			HitDiavolo->GetCharacterMovement()->AddImpulse(GetActorUpVector() * BasicAttack_Strength / Distance, true);
			HitDiavolo->SetbIsDamagedTrue();
			HitDiavolo->PlayDiavoloRandomDeadMontage();
		}
	}
}


void AOPGalio::BasicAttack_Trace_Right() const
{
TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			float Distance = FVector::Dist(GetActorLocation(), HitDiavolo->GetActorLocation());
			HitDiavolo->GetCharacterMovement()->AddImpulse(-GetActorRightVector() * BasicAttack_Strength / Distance, true);
			HitDiavolo->SetbIsDamagedTrue();
			HitDiavolo->PlayDiavoloRandomDeadMontage();
		}
	}
}


void AOPGalio::BasicAttack_Trace_Left() const
{
TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			float Distance = FVector::Dist(GetActorLocation(), HitDiavolo->GetActorLocation());
			HitDiavolo->GetCharacterMovement()->AddImpulse(GetActorRightVector() * BasicAttack_Strength / Distance, true);
			HitDiavolo->SetbIsDamagedTrue();
			HitDiavolo->PlayDiavoloRandomDeadMontage();
		}
	}
}


void AOPGalio::Q()
{
	Super::Q();

	if (!bQ) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);
	Q_FinalLocation = MouseCursorHit.Location;
	// Q_FinalLocation.Z += 1000.f;

	GetWorldTimerManager().SetTimer(Q_Cast_TimerHandle, this, &AOPGalio::Q_SpawnWindblast, 0.25f, false);

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Q_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPGalio::ResetChampionMovement, 0.7f, false);

	SetbQ_False();
	GetWorldTimerManager().SetTimer(Q_Cooldown_TimerHandle, this, &AOPGalio::SetbQ_True, Q_Cooldown, false);
}

void AOPGalio::Q_SpawnWindblast()
{
	if (Q_Windblast_Class == nullptr) return;
	
	Q_Windblast_Storage_1 = GetWorld()->SpawnActor<AOPGalioWindblast>(Q_Windblast_Class, Q_Windblast_SpawnPoint_1->GetComponentLocation(), Q_Windblast_SpawnPoint_1->GetComponentRotation());
	Q_Windblast_Storage_1->SetOwner(this);
	Q_Windblast_Storage_2 = GetWorld()->SpawnActor<AOPGalioWindblast>(Q_Windblast_Class, Q_Windblast_SpawnPoint_2->GetComponentLocation(), Q_Windblast_SpawnPoint_2->GetComponentRotation());
	Q_Windblast_Storage_2->SetOwner(this);

	FVector WindblastVector1 = FVector::ZeroVector;
	FVector WindblastVector2 = FVector::ZeroVector;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, WindblastVector1, Q_Windblast_SpawnPoint_1->GetComponentLocation(), Q_FinalLocation, GetWorld()->GetGravityZ(), 0.7f);
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, WindblastVector2, Q_Windblast_SpawnPoint_2->GetComponentLocation(), Q_FinalLocation, GetWorld()->GetGravityZ(), 0.7f);
	Q_Windblast_Storage_1->GetOPProjectileMovementComponent()->Velocity = WindblastVector1;
	Q_Windblast_Storage_2->GetOPProjectileMovementComponent()->Velocity = WindblastVector2;
	
}

void AOPGalio::W()
{
	Super::W();

	if(!bW) return;

	if(!bW_IsCharging)
	{
		bW_IsCharging = true;
		W_Charge();

		GetWorldTimerManager().SetTimer(W_CastTimer, this, &AOPGalio::W_Recast, 2.5f, false);

		if (W_NiagaraSystemComponent_Charging)
		{
			W_NiagaraSystemComponent_Charging->Activate();
		}
	}

	else if(bW_IsCharging)
	{
		W_Recast();
	}
}

void AOPGalio::W_Charge()
{
	if(W_ChargeCurve)
	{
		FOnTimelineFloat W_TimelineFloat;
		W_TimelineFloat.BindUFunction(this, FName(TEXT("W_TimelineProgress")));

		W_Timeline.AddInterpFloat(W_ChargeCurve, W_TimelineFloat);

		W_Timeline.PlayFromStart();
	}
}

void AOPGalio::W_Recast()
{
	bW_IsCharging = false;
	W_Timeline.Stop();
	GetWorldTimerManager().ClearTimer(W_CastTimer);
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
		GetActorLocation(), W_FinalRange,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				FVector PullVector = (GetActorLocation() - HitDiavolo->GetActorLocation()).GetSafeNormal();
				HitDiavolo->GetCharacterMovement()->AddImpulse(PullVector * W_PullStrength, true);
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
			}
		}
	}
	
	if(ChampionAnimInstance && W_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(W_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection(FName(TEXT("Recast")), W_AnimMontage);
	}

	if (W_NiagaraSystemComponent_Charging)
	{
		W_NiagaraSystemComponent_Charging->DeactivateImmediate();
	}

	if (W_NiagaraSystem_Recast)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), W_NiagaraSystem_Recast, GetActorLocation());
	}
	
	SetbW_False();
	GetWorldTimerManager().SetTimer(W_Cooldown_TimerHandle, this, &AOPGalio::SetbW_True, W_Cooldown, false);
}

void AOPGalio::W_TimelineProgress(float Value)
{
	if(bW_IsCharging)
	{
		float CurrentDistance = FMath::Lerp(W_StartRange, W_EndRange, Value);
		W_FinalRange = CurrentDistance;
	}
}

void AOPGalio::E()
{
	Super::E();

	if (!bE) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(E_DashStart_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		bE_IsDashing = true;
		LaunchCharacter(E_Speed * GetActorForwardVector(), true, true);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	}), 0.5f, false);

	GetWorldTimerManager().SetTimer(E_DashEnd_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		bE_IsDashing = false;
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	}), 1.5f, false);
	
	LaunchCharacter(-E_Speed_Backward * GetActorForwardVector(), true, true);

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection("Dash", E_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPGalio::ResetChampionMovement, 1.5f, false);

	SetbE_False();
	GetWorldTimerManager().SetTimer(E_Cooldown_TimerHandle, this, &AOPGalio::SetbE_True, E_Cooldown, false);
}

void AOPGalio::E_Trace() const
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), E_Radius, 3000.f,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			// bE_IsDashing = false;
			FRotator ImpactDirection = (HitDiavolo->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
			ImpactDirection.Pitch = E_Angle;
			
			HitDiavolo->GetCharacterMovement()->AddImpulse((GetActorUpVector() + ImpactDirection.Vector()) * E_Strength, true);
			HitDiavolo->SetbIsDamagedTrue();
			HitDiavolo->PlayDiavoloRandomDeadMontage();

			if(ChampionAnimInstance && E_AnimMontage)
			{
				ChampionAnimInstance->Montage_Play(E_AnimMontage);
				ChampionAnimInstance->Montage_JumpToSection(FName(TEXT("Hit")), E_AnimMontage);
			}
		}
	}
}

void AOPGalio::R()
{
	Super::R();

	if (!bR) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	R_FinalLocation = MouseCursorHit.Location;

	GetWorldTimerManager().SetTimer(R_Departure_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		bR_IsInAir = true;
		LaunchCharacter(R_Speed_Z_Departure * GetActorUpVector(), true, true);
		if (R_NiagaraSystem_Departure)
		{
			FVector NiagaraSpawnLocation = GetActorLocation();
			NiagaraSpawnLocation.Z -= 2400.f;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), R_NiagaraSystem_Departure, NiagaraSpawnLocation);
		}
	}), 1.f, false);

	GetWorldTimerManager().SetTimer(R_SpawnNiagara_InAir_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		if (R_NiagaraSystem_InAir)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), R_NiagaraSystem_InAir, GetActorLocation());
			GetCharacterMovement()->StopMovementImmediately();
		}
	}), 1.5f, false);

	GetWorldTimerManager().SetTimer(R_StartLanding_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		GetCharacterMovement()->AddForce(-R_Speed_Z_Landing * GetActorUpVector());
		LaunchCharacter(-R_Speed_Z_Landing * GetActorUpVector() + R_Speed_XY_Landing * GetActorForwardVector(), true, true);

		if (R_NiagaraSystem_SonicBoom)
		{
			FRotator NiagaraSpawnRotation = GetActorRotation();
			NiagaraSpawnRotation.Pitch = 60.f;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), R_NiagaraSystem_SonicBoom,
				GetActorLocation() + GetActorForwardVector() * 2000.f, NiagaraSpawnRotation);
			GetCharacterMovement()->GravityScale = R_Gravity;
		}
	}), 1.9f, false);

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection("Start", R_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPGalio::ResetChampionMovement, 3.5f, false);

	SetbR_False();
	GetWorldTimerManager().SetTimer(R_Cooldown_TimerHandle, this, &AOPGalio::SetbR_True, R_Cooldown, false);
}

void AOPGalio::R_OnLanding()
{
	bR_IsInAir = false;
	GetCharacterMovement()->GravityScale = 1.f;
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), R_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			FRotator AirborneDirection = (HitDiavolo->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
			AirborneDirection.Pitch = R_Angle;
			HitDiavolo->GetCharacterMovement()->AddImpulse((GetActorUpVector() + AirborneDirection.Vector()) * R_Strength, true);
			HitDiavolo->SetbIsDamagedTrue();
			HitDiavolo->PlayDiavoloRandomDeadMontage();
		}
	}

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection("Land", R_AnimMontage);
	}

	GetWorldTimerManager().SetTimer(R_SpawnNiagara_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		if (R_NiagaraSystem_Land)
		{
			FVector NiagaraSpawnLocation = GetActorLocation();
			NiagaraSpawnLocation.Z -= 2400.f;
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), R_NiagaraSystem_Land, NiagaraSpawnLocation);
		}
	}), 0.05f, false);
}
