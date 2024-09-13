// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Riven/OPRiven.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/Riven/OPRivenSlash.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"

AOPRiven::AOPRiven()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");

	E_ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>("E_ShieldMesh");
	E_ShieldMesh->SetupAttachment(GetRootComponent());
	
	E_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("E Niagara Component");
	E_NiagaraComponent->SetupAttachment(E_ShieldMesh);

	R_SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sword Mesh");
	R_SwordMesh->SetupAttachment(GetMesh(), FName(TEXT("R Sword Socket")));
	R_SwordMesh->SetHiddenInGame(true);

	R_SlashSpawnPoints.Add(CreateDefaultSubobject<USceneComponent>("Spawn Point 1"));
	R_SlashSpawnPoints.Add(CreateDefaultSubobject<USceneComponent>("Spawn Point 2"));
	R_SlashSpawnPoints.Add(CreateDefaultSubobject<USceneComponent>("Spawn Point 3"));
	
	for (auto& Point : R_SlashSpawnPoints)
	{
		Point->SetupAttachment(GetRootComponent());
	}
}

void AOPRiven::BeginPlay()
{
	Super::BeginPlay();

	E_ShieldMesh->SetHiddenInGame(true);
	E_ShieldMesh->SetCollisionObjectType(ECC_WorldStatic);
	E_ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	E_ShieldMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	E_NiagaraComponent->SetHiddenInGame(true);
}

void AOPRiven::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bQ_IsInAir)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			Q_OnLanding();
			bQ_IsInAir = false;
		}
	}
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
			GetWorldTimerManager().SetTimer(Q_Trace_TimerHandle, this, &AOPRiven::Q_Trace, 0.25f, false);
			Q_Step++;
			break;
		case 2:
			ChampionAnimInstance->Montage_JumpToSection(FName("Q_2"), Q_AnimMontage);
			LaunchCharacter(GetActorForwardVector() * Q_Speed_XY, true, true);
			GetWorldTimerManager().SetTimer(Q_Trace_TimerHandle, this, &AOPRiven::Q_Trace, 0.25f, false);
			Q_Step++;
			break;
		case 3:
			ChampionAnimInstance->Montage_JumpToSection(FName("Q_3"), Q_AnimMontage);
			bQ_IsInAir = true;
			LaunchCharacter(GetActorForwardVector() * Q_Speed_XY_ThirdShot + GetActorUpVector() * Q_Speed_Z, true, true);
			GetWorldTimerManager().SetTimer(Q_Trace_TimerHandle, this, &AOPRiven::Q_Trace_Third, 0.4f, false);
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

	FRotator RivenRotator = GetActorRotation();
	RivenRotator.Yaw = 40.f;
	// RivenRotator.Pitch = 50.f;
	// RivenRotator.Roll = 90.f;

	if (Q_NiagaraSystem_Slash)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Q_NiagaraSystem_Slash, GetActorLocation(), RivenRotator);
	}
	
	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Q_NiagaraSystem_Hit, Diavolo->GetActorLocation());
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

	FRotator RivenRotator = GetActorRotation();
	// RivenRotator.Yaw = 40.f;
	RivenRotator.Pitch = 50.f;
	RivenRotator.Roll = 90.f;
	if (Q_NiagaraSystem_Slash)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Q_NiagaraSystem_Slash, GetActorLocation(), RivenRotator);
	}
	
	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Q_NiagaraSystem_Hit, Diavolo->GetActorLocation());
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

void AOPRiven::Q_OnLanding()
{
	if (Q_NiagaraSystem_OnLanding)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z -= 90.f;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Q_NiagaraSystem_OnLanding, SpawnLocation);
	}
}

void AOPRiven::W()
{
	Super::W();

	if (!bW) return;

	GetWorldTimerManager().SetTimer(W_Cast_TimerHandle, this, &AOPRiven::W_Trace, 0.25f, false);
	
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

	if (W_NiagaraSystem)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z -= 90.f;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), W_NiagaraSystem, SpawnLocation);
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

	E_NiagaraComponent->SetHiddenInGame(false);
	E_ShieldMesh->SetHiddenInGame(false);
	E_ShieldMesh->SetCollisionObjectType(ECC_WorldDynamic);
	E_ShieldMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	E_ShieldMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	E_ShieldMesh->OnComponentBeginOverlap.AddDynamic(this, &AOPRiven::E_Reflect);
	GetWorldTimerManager().SetTimer(E_ShieldMaintain_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		E_NiagaraComponent->SetHiddenInGame(true);
		E_ShieldMesh->SetHiddenInGame(true);
		E_ShieldMesh->SetCollisionObjectType(ECC_WorldDynamic);
		E_ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		E_ShieldMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		E_ShieldMesh->OnComponentBeginOverlap.RemoveDynamic(this, &AOPRiven::E_Reflect);
	}), E_MaintainTime, false);

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPRiven::ResetChampionMovement, 0.1f, false);
	SetbE_False();
	GetWorldTimerManager().SetTimer(E_Cooldown_TimerHandle, this, &AOPRiven::SetbE_True, E_Cooldown, false);
}

void AOPRiven::E_Reflect(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Pverla[[ed");
		AOPProjectile* OtherProjectile = Cast<AOPProjectile>(OtherActor);
		if (OtherProjectile)
		{
			FRotator ImpactRotation = (OtherProjectile->GetActorLocation() - E_ShieldMesh->GetComponentLocation()).GetSafeNormal().Rotation();
			FRotator FinalRotation = FRotator(E_ReflectAngle, ImpactRotation.Yaw, ImpactRotation.Roll);
			FVector ImpactDirection = FinalRotation.Vector();
            
			UProjectileMovementComponent* ProjectileMovement = OtherProjectile->FindComponentByClass<UProjectileMovementComponent>();
			if (ProjectileMovement)
			{
				ProjectileMovement->Velocity = ImpactDirection * W_Strength; // 투사체의 속도를 충격 방향으로 설정
			}
		}
		//DestroyProjectile();
	}
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
		StopChampionMovement();
		GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPRiven::ResetChampionMovement, 0.35f, false);
	}

	else if (bR_CanSlash)
	{
		bR_CanSlash = false;
		if (OPPlayerController == nullptr) return;
		OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

		if (!MouseCursorHit.bBlockingHit) return;
		TurnCharacterToCursor(MouseCursorHit);

		GetWorldTimerManager().SetTimer(R_Slash_TimerHandle, this, &AOPRiven::R_Slash, 0.25f, false);

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

void AOPRiven::R_Slash()
{
	if (R_SlashClass == nullptr) return;
	for (int32 i = 0; i < 3; i++)
	{
		R_SlashStorages.Add(GetWorld()->SpawnActor<AOPRivenSlash>(R_SlashClass, R_SlashSpawnPoints[i]->GetComponentLocation(), R_SlashSpawnPoints[i]->GetComponentRotation()));
		R_SlashStorages[i]->SetOwner(this);
	}

	FRotator RivenRotator = GetActorRotation();
	RivenRotator.Yaw = 40.f;
	if (Q_NiagaraSystem_Slash)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Q_NiagaraSystem_Slash, GetActorLocation(), RivenRotator);
	}
}
