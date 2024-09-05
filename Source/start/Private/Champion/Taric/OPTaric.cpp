// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Taric/OPTaric.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/Taric/OPTaricShield.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"
#include "Projectile/OPProjectile.h"

AOPTaric::AOPTaric()
{
	W_Shield_GeometryCollection = CreateDefaultSubobject<UOPTaricShield>("W Shield Geometry Collection");
	W_Shield_GeometryCollection->SetupAttachment(GetRootComponent());

	FragilePoint = CreateDefaultSubobject<UBoxComponent>("FragilePoint");
	FragilePoint->SetupAttachment(W_Shield_GeometryCollection);
}

void AOPTaric::BeginPlay()
{
	Super::BeginPlay();

	FragilePoint->OnComponentBeginOverlap.AddDynamic(this, &AOPTaric::OnFragileCollisionBeginOverlap);
}

void AOPTaric::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOPTaric::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;
	if (OPPlayerController == nullptr) return;
	
	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		if (bPassive_IsOn)
		{
			GetWorldTimerManager().SetTimer(BasicAttack_Trace_Passive_TimerHandle, FTimerDelegate::CreateLambda([&]
			{
				BasicAttack_Trace_Passive();
			}), 0.3f, false);

			bPassive_IsOn = false;
			ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);

			switch (BasicAttack_ComboCount_Passive) // 2번의 연결된 평타동작
			{
			case 0:
				ChampionAnimInstance->Montage_JumpToSection(FName("Passive_1"), BasicAttack_AnimMontage);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_Passive_TimerHandle, this, &AOPTaric::BasicAttack_ResetComboCount_Passive, 5.f, false);
				BasicAttack_ComboCount_Passive++;
				break;

			case 1:
				ChampionAnimInstance->Montage_JumpToSection(FName("Passive_2"), BasicAttack_AnimMontage);
				GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_Passive_TimerHandle);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_Passive_TimerHandle, this, &AOPTaric::BasicAttack_ResetComboCount_Passive, 5.f, false);
				BasicAttack_ComboCount_Passive = 0;
				break;
				
			default:
				;
			}
		}

		else
		{
			GetWorldTimerManager().SetTimer(BasicAttack_Trace_TimerHandle, FTimerDelegate::CreateLambda([&]
			{
				BasicAttack_Trace();
			}), 0.3f, false);

			ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
			switch (BasicAttack_ComboCount) // 2번의 연결된 평타동작
			{
			case 0:
				ChampionAnimInstance->Montage_JumpToSection("1", BasicAttack_AnimMontage);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPTaric::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount++;
				break;

			case 1:
				ChampionAnimInstance->Montage_JumpToSection("2", BasicAttack_AnimMontage);
				GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_TimerHandle);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPTaric::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount++;
				break;

			case 2:
				ChampionAnimInstance->Montage_JumpToSection("3", BasicAttack_AnimMontage);
				GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_TimerHandle);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPTaric::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount = 0;
				break;
				
			default:
				;
			}
		}
	}
	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTaric::ResetChampionMovement, 1.7f, false);
	
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPTaric::SetbBasicAttack_True, BasicAttack_Cooldown, false);
}

void AOPTaric::BasicAttack_Trace()
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

void AOPTaric::BasicAttack_Trace_Passive()
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
		Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * BasicAttack_Passive_Strength, true);
		if (!Diavolo->GetbCanBeTestedMultipleTimes())
		{
			Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
	}
}

void AOPTaric::Q()
{
	Super::Q();

	if (!bQ) return;
	if (OPPlayerController == nullptr) return;

	bPassive_IsOn = true;

	if (ChampionAnimInstance && Q_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Q_AnimMontage);
	}
	
	FVector NiagaraSpawnLocation = GetActorLocation();
	NiagaraSpawnLocation.Z -= 90.f;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Q_NiagaraSystem, NiagaraSpawnLocation);

	if (W_TargetAlly && bW_IsTethered)
	{
		FVector AllyLocation = W_TargetAlly->GetActorLocation();
		AllyLocation.Z -= 90.f;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Q_NiagaraSystem, AllyLocation, FRotator::ZeroRotator, FVector(2.f));
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTaric::ResetChampionMovement, 1.f, false);
	
	SetbQ_False();
	GetWorldTimerManager().SetTimer(Q_Cooldown_TimerHandle, this, &AOPTaric::SetbQ_True, Q_Cooldown, false);
}

void AOPTaric::Q_Trace()
{
}

void AOPTaric::W()
{
	Super::W();
	
	if (!bW) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	bPassive_IsOn = true;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), W_Shield_NiagaraSystem, GetActorLocation());
	AActor* ShieldMesh = GetWorld()->SpawnActor<AActor>(W_ShieldMesh, GetActorLocation(), GetActorRotation());
	ShieldMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	if (W_TargetAlly = Cast<AOPChampion>(MouseCursorHit.GetActor()))
	{
		bW_IsTethered = true;
		AActor* AllyShieldMesh = GetWorld()->SpawnActor<AActor>(W_ShieldMesh, W_TargetAlly->GetActorLocation(), GetActorRotation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), W_Shield_NiagaraSystem, W_TargetAlly->GetActorLocation());
	}

	if (ChampionAnimInstance && W_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(W_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTaric::ResetChampionMovement, 1.2f, false);
	
	SetbW_False();
	GetWorldTimerManager().SetTimer(W_Cooldown_TimerHandle, this, &AOPTaric::SetbW_True, W_Cooldown, false);
}

void AOPTaric::OnFragileCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (AOPProjectile* Projectile = Cast<AOPProjectile>(OtherActor))
		{
			W_Shield_GeometryCollection->SetDamageThreshold(CustomDamageThreshold);
			GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Fragiled");
		}
	}
}

void AOPTaric::E()
{
	Super::E();

	if (!bE) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	bPassive_IsOn = true;

	GetWorldTimerManager().SetTimer(E_Trace_TimerHandle_Taric, FTimerDelegate::CreateLambda([&]
	{
		E_Trace(GetActorLocation(), GetActorForwardVector());
	}), 1.f, false);

	if (W_TargetAlly && bW_IsTethered)
	{
		FVector AllyLocation = W_TargetAlly->GetActorLocation();
		E_AllyVectorFor_Trace = MouseCursorHit.Location - AllyLocation;
		E_AllyVectorFor_Trace.Z = 0.f;
		E_AllyVectorFor_Trace.Normalize();

		GetWorldTimerManager().SetTimer(E_Trace_TimerHandle_Ally, FTimerDelegate::CreateLambda([&]
		{
			FVector FinalAllyLocation = W_TargetAlly->GetActorLocation();
			E_Trace(FinalAllyLocation, E_AllyVectorFor_Trace);
			UE_LOG(LogTemp, Warning, TEXT("Hello"));
		}), 1.f, false);
	}

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTaric::ResetChampionMovement, 1.5f, false);
	
	SetbE_False();
	GetWorldTimerManager().SetTimer(E_Cooldown_TimerHandle, this, &AOPTaric::SetbE_True, E_Cooldown, false);
}

void AOPTaric::E_Trace(const FVector& StartLocation, const FVector& Direction)
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	if (W_TargetAlly)
	{
		ActorsToIgnore.Add(W_TargetAlly);
	}

	if (bE_DrawDebugTrace)
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StartLocation, StartLocation + Direction * E_Range, E_Width,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);
	}

	else
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StartLocation, StartLocation + Direction * E_Range, E_Width,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	}

	for (auto& HitResult : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitResult.GetActor()))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->GetChampionAnimInstance()->Montage_Play(Diavolo->GetDiavolo_DamagedByTaric_E_AnimMontage());
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), E_NiagaraSystem, StartLocation, Direction.Rotation());
}

void AOPTaric::R()
{
	Super::R();
	
	if (!bR) return;
	if (OPPlayerController == nullptr) return;

	bPassive_IsOn = true;

	UNiagaraFunctionLibrary::SpawnSystemAttached(R_Casting_NiagaraSystem, GetCapsuleComponent(), NAME_None, GetActorLocation(), FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
	if (R_BallClass)
	{
		FVector BallSpawnPoint = GetActorLocation();
		BallSpawnPoint.Z += 1000.f;
		GetWorld()->SpawnActor<AActor>(R_BallClass, BallSpawnPoint, GetActorRotation());
	}
	GetWorldTimerManager().SetTimer(R_Trace_TimerHandle_Taric, FTimerDelegate::CreateLambda([&]
	{
		FVector TaricLocation = GetActorLocation();
		R_Trace(GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAttached(R_Shield_NiagaraSystem, GetCapsuleComponent(), NAME_None, TaricLocation, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
	}), 2.5f, false);

	if (W_TargetAlly && bW_IsTethered)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(R_Casting_NiagaraSystem, W_TargetAlly->GetCapsuleComponent(), NAME_None, W_TargetAlly->GetActorLocation(), FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
		if (R_BallClass)
		{
			FVector BallSpawnPoint = GetActorLocation();
			BallSpawnPoint.Z += 1000.f;
			GetWorld()->SpawnActor<AActor>(R_BallClass, BallSpawnPoint, GetActorRotation());
		}
		GetWorldTimerManager().SetTimer(R_Trace_TimerHandle_Ally, FTimerDelegate::CreateLambda([&]
		{
			FVector AllyLocation = W_TargetAlly->GetActorLocation();
			UNiagaraFunctionLibrary::SpawnSystemAttached(R_Shield_NiagaraSystem, W_TargetAlly->GetCapsuleComponent(), NAME_None, AllyLocation, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
		}), 2.5f, false);
	}

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTaric::ResetChampionMovement, 1.f, false);
	
	SetbR_False();
	GetWorldTimerManager().SetTimer(R_Cooldown_TimerHandle, this, &AOPTaric::SetbR_True, R_Cooldown, false);
}

void AOPTaric::R_Trace(const FVector& StartLocation)
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	if (bR_DrawDebugTrace)
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StartLocation, StartLocation, R_Radius,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, false);
	}

	else
	{
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), StartLocation, StartLocation, R_Radius,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, false);
	}

	UNiagaraFunctionLibrary::SpawnSystemAttached(R_Burst_NiagaraSystem, GetCapsuleComponent(), NAME_None, StartLocation, FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, true);
}
