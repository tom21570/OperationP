// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Malphite/OPMalphite.h"
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"
#include "Champion/Malphite/OPMalphiteShardOfTheEarth.h"

AOPMalphite::AOPMalphite()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	Q_ShardSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Shard Spawn Location"));
	Q_ShardSpawnLocation->SetupAttachment(GetRootComponent());

	W_ClapPoint = CreateDefaultSubobject<USceneComponent>(TEXT("W_ClapPoint Component"));
	W_ClapPoint->SetupAttachment(GetRootComponent());
}

void AOPMalphite::BeginPlay()
{
	Super::BeginPlay();
}

void AOPMalphite::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bR_IsCasting)
	{
		R_Trace();
	}
}

void AOPMalphite::Passive()
{
	Super::Passive();
}

void AOPMalphite::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;
	if (!OPPlayerController) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);	
	
	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
		
		if (bW_ThunderClapOn == true)
		{
			ChampionAnimInstance->Montage_JumpToSection(FName("clap"), BasicAttack_AnimMontage);
			GetWorldTimerManager().SetTimer(BasicAttack_Trace_TimerHandle, FTimerDelegate::CreateLambda([&]
			{
				BasicAttackTrace_W();
			}), 0.45f, false);
			bW_ThunderClapOn = false;
		}
		
		else
		{
			ChampionAnimInstance->Montage_JumpToSection(FName("1"), BasicAttack_AnimMontage);
			GetWorldTimerManager().SetTimer(BasicAttack_Trace_TimerHandle, FTimerDelegate::CreateLambda([&]
			{
				BasicAttackTrace();
			}), 0.4f, false);
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPMalphite::ResetChampionMovement, 0.6f, false);
	
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPMalphite::SetbBasicAttack_True, GetBasicAttackCooltime(), false);
}

void AOPMalphite::BasicAttackTrace()
{
	TArray<FHitResult> HitEnemies;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitEnemies, true);
	
	for (auto& HitActor : HitEnemies)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			float Distance = FVector::Dist(GetActorLocation(), Diavolo->GetActorLocation());
			Diavolo->LaunchCharacter(GetActorRightVector() * BasicAttack_Strength / Distance, true, true);
			
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
}

void AOPMalphite::BasicAttackTrace_W()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), W_ClapPoint->GetComponentLocation(), W_ClapPoint->GetComponentLocation(), BasicAttack_Width_W,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (Diavolo)
			{
				const float Distance = FVector::Dist(GetActorLocation(), Diavolo->GetActorLocation());
				FVector ImpulseDirection = Diavolo->GetActorLocation() - W_ClapPoint->GetComponentLocation();
				Diavolo->LaunchCharacter(ImpulseDirection * BasicAttack_Strength_W / Distance, true, true);
				
				Diavolo->SetbIsDamagedTrue();
				Diavolo->PlayDiavoloRandomDeadMontage();
				
				if (!Diavolo->GetbCanBeTestedMultipleTimes())
				{
					Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
				}
			}
		}
	}
}

void AOPMalphite::Q()
{
	if (!bQ) return;
	if (!OPPlayerController) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToLocation(MouseCursorHit.Location);
	
	UE_LOG(LogTemp, Log, TEXT("Skill_1_ShardOfTheEarth"));
	ChampionAnimInstance->Montage_Play(Q_AnimMontage, 1.0f);

	GetWorldTimerManager().SetTimer(Q_Spawn_TimerHandle, this, &AOPMalphite::Q_ShardOfTheEarth, 0.25f, false);

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPMalphite::ResetChampionMovement, 0.9f, false);
	SetbQ_False();
	GetWorldTimerManager().SetTimer(Q_Cooldown_TimerHandle, this, &AOPMalphite::SetbQ_True, GetQ_Cooldown(), false);
}

void AOPMalphite::ApplySkill_1_Effect(AOPChampion* SourceChampion, AOPDiavolo* OtherChampion)
{
	float OriginalSpeed = OtherChampion->GetCharacterMovement()->MaxWalkSpeed;

	float SlowedSpeed = OriginalSpeed * 0.8f;
	OtherChampion->GetCharacterMovement()->MaxWalkSpeed = SlowedSpeed;

	if (AOPMalphite* SourceCharacter = Cast<AOPMalphite>(SourceChampion))
	{
		float SpeedIncrease = OriginalSpeed - SlowedSpeed;
		SourceCharacter->GetCharacterMovement()->MaxWalkSpeed += SpeedIncrease;
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, [this, SourceCharacter, OriginalSpeed, SpeedIncrease]()
		{
			GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
			
			SourceCharacter->GetCharacterMovement()->MaxWalkSpeed -= SpeedIncrease;
		}, Q_SlowDuration, false);
	}
}

void AOPMalphite::SetMalphite_HP_Damaged(float Force, AOPMalphite* Malphite)
{
	float curHp = Malphite->GetMalphite_HP();
	curHp -= Force;
	if (curHp < 0) Malphite->SetMalphite_HP(0);
	else Malphite->SetMalphite_HP(curHp);

	UE_LOG(LogTemp, Warning, TEXT("Malphite HP: %f"), GetMalphite_HP());
}

float AOPMalphite::GetMalphite_HP()
{
	return HP;
}

void AOPMalphite::SetMalphite_HP(float hp)
{
	HP = hp;
}

void AOPMalphite::Q_ShardOfTheEarth()
{
	if (Q_ShardOfTheEarthClass == nullptr) return;

	Q_ShardOfTheEarthStorage = GetWorld()->SpawnActor<AOPMalphiteShardOfTheEarth>(Q_ShardOfTheEarthClass, Q_ShardSpawnLocation->GetComponentLocation(), GetActorRotation());
	Q_ShardOfTheEarthStorage->SetOwner(this);
}

void AOPMalphite::W()
{
	Super::W();

	bW_ThunderClapOn = true;

	if (ChampionAnimInstance && W_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(W_AnimMontage, 1.f);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPMalphite::ResetChampionMovement, 0.55f, false);

	SetbW_False();
	GetWorldTimerManager().SetTimer(W_Cooldown_TimerHandle, this, &AOPMalphite::SetbW_True, W_Cooldown, false);
}


void AOPMalphite::E()
{
	Super::E();

	if (!bE) return;
	if (!OPPlayerController) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	GetWorldTimerManager().SetTimer(E_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		E_GroundSlam();
	}), 0.25f, false);

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage, 1.f);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPMalphite::ResetChampionMovement, 0.3f, false);

	SetbE_False();
	GetWorldTimerManager().SetTimer(E_Cooldown_TimerHandle, this, &AOPMalphite::SetbE_True, GetE_Cooldown(), false);
}


void AOPMalphite::E_GroundSlam()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * E_Radius, E_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

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
}

void AOPMalphite::R()
{
	Super::R();

	if (!bR) return;
	if (!OPPlayerController) return;
	
	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage, 1.0f);
	}
	
	R_FinalLocation = MouseCursorHit.Location;
	TurnCharacterToLocation(R_FinalLocation);
	FVector LaunchVector = R_FinalLocation - GetActorLocation();
	LaunchVector.Normalize();
	LaunchVector.Z = 0.f;
	const float FinalDistance = (R_FinalLocation - GetActorLocation()).Length();
	
	// GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AOPMalphite::R_OnProjectileHit);
	ProjectileMovementComponent->Velocity = LaunchVector * R_Speed;
	bR_IsCasting = true;
	
	GetWorldTimerManager().SetTimer(R_Stop_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		bR_IsCasting = false;
		// GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &AOPMalphite::R_OnProjectileHit);
		ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	}), FinalDistance / R_Speed, false);
	
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), FinalDistance);
	

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPMalphite::ResetChampionMovement, 0.3f, false);
	
	SetbR_False();
	GetWorldTimerManager().SetTimer(R_Cooldown_TimerHandle, this, &AOPMalphite::SetbR_True, GetR_Cooldown(), false);
}

void AOPMalphite::R_Trace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * R_Radius, R_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				FVector ImpactDirection = (HitDiavolo->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				ImpactDirection.Z = 0.f;
				ImpactDirection.Z = R_Angle;
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
				HitDiavolo->LaunchCharacter(ImpactDirection * R_Strength, true, true);
				// HitDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * R_Impulse, true);
			}
		}
	}
}

void AOPMalphite::R_OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnProjectileHit Active 1"));
		UE_LOG(LogTemp, Warning, TEXT("HitComponent: %s / OtherActor: %s / OtherComp: %s / Hit: %s"),
			*HitComponent->GetName(),
			*OtherActor->GetName(),
			*OtherComp->GetName(),
			*Hit.ToString());
		UE_LOG(LogTemp, Warning, TEXT("OnProjectileHit Active 2"));

		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(OtherActor))
		{            // Calculate the direction of the impulse
			FVector ImpactDirection = (Diavolo->GetActorLocation() - Hit.ImpactPoint).GetSafeNormal();

			// Add an upward component to the impact direction
			ImpactDirection.Z = R_Angle;
			ImpactDirection = ImpactDirection.GetSafeNormal();

			// Log the impact direction for debugging
			UE_LOG(LogTemp, Log, TEXT("Impact Direction: %s"), *ImpactDirection.ToString());

			// Apply an impulse to the Diavolo character based on the impact direction and AirborneRate
			Diavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * R_Strength, true);

			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}
		}
	}
}
