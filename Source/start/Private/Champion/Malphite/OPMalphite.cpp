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

	ShardSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Shard Spawn Location"));
	ShardSpawnLocation->SetupAttachment(GetRootComponent());
}

void AOPMalphite::BeginPlay()
{
	Super::BeginPlay();
}

void AOPMalphite::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bRIsCasting)
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

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]
	{
			BasicAttackTrace();
	}), 0.25f, false);

	if (ChampionAnimInstance && BasicAttackAnimMontage)
	{
		if (bThunderClapOn == true)
		{
			ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection(FName("clap"), BasicAttackAnimMontage);
			bThunderClapOn = false;
		}
		else
		{
			ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
		}
		
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPMalphite::ResetChampionMovement, 0.6f, false);
	
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttackCooltimeTimerHandle, this, &AOPMalphite::SetbBasicAttack_True, GetBasicAttackCooltime(), false);
}

bool AOPMalphite::BasicAttackTrace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f, 80.f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
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

void AOPMalphite::Q()
{
	if (!bQ) return;
	if (!OPPlayerController) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToLocation(MouseCursorHit.Location);
	
	UE_LOG(LogTemp, Log, TEXT("Skill_1_ShardOfTheEarth"));
	ChampionAnimInstance->Montage_Play(Q_AnimMontage, 1.0f);

	GetWorldTimerManager().SetTimer(ShardOfTheEarthSpawnTimer, this, &AOPMalphite::Q_ShardOfTheEarth, 0.25f, false);

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPMalphite::ResetChampionMovement, 0.9f, false);
	SetbQ_False();
	GetWorldTimerManager().SetTimer(Q_CooldownTimerHandle, this, &AOPMalphite::SetbQ_True, GetQ_Cooldown(), false);
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
		}, Skill_1_SlowDuration, false);
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
	if (ShardOfTheEarthClass == nullptr) return;

	ShardOfTheEarth = GetWorld()->SpawnActor<AOPMalphiteShardOfTheEarth>(ShardOfTheEarthClass, ShardSpawnLocation->GetComponentLocation(), GetActorRotation());
	ShardOfTheEarth->SetOwner(this);
}

void AOPMalphite::W()
{
	Super::W();

	bThunderClapOn = true;

	if (ChampionAnimInstance && W_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(W_AnimMontage, 1.f);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPMalphite::ResetChampionMovement, 0.55f, false);

	SetbW_False();
	GetWorldTimerManager().SetTimer(W_CooldownTimerHandle, this, &AOPMalphite::SetbW_True, W_Cooldown, false);
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

	GetWorldTimerManager().SetTimer(E_CastTimer, FTimerDelegate::CreateLambda([&]
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
	GetWorldTimerManager().SetTimer(E_CooldownTimerHandle, this, &AOPMalphite::SetbE_True, GetE_Cooldown(), false);
}


void AOPMalphite::E_GroundSlam()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * Skill_3_Radius, Skill_3_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				FVector MalphiteLocation = FVector(GetActorLocation().X, GetActorLocation().Y, 0.f);
				FVector ImpulseDirection = (HitDiavolo->GetActorLocation() - MalphiteLocation).GetSafeNormal();
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
				// HitDiavolo->LaunchCharacter(ImpulseDirection * Skill_3_Impulse, true, true);
				HitDiavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 100000.f, true);
				HitDiavolo->ApplySlowAttackEffect(Skill_3_SlowAmount, Skill_3_SlowDuration); //��ƺ��ο� ���ݸ���� �������� �Լ� �����ʿ�
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
	bRIsCasting = true;
	
	GetWorldTimerManager().SetTimer(R_StopTimer, FTimerDelegate::CreateLambda([&]
	{
		bRIsCasting = false;
		// GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &AOPMalphite::R_OnProjectileHit);
		ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	}), FinalDistance / R_Speed, false);
	
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), FinalDistance);
	

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPMalphite::ResetChampionMovement, 0.3f, false);
	
	SetbR_False();
	GetWorldTimerManager().SetTimer(R_CooldownTimerHandle, this, &AOPMalphite::SetbR_True, GetR_Cooldown(), false);
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
				ImpactDirection.Z += R_Angle;
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
				HitDiavolo->LaunchCharacter(ImpactDirection * R_Impulse, true, true);
				// HitDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * R_Impulse, true);
				HitDiavolo->ApplySlowAttackEffect(Skill_3_SlowAmount, Skill_3_SlowDuration); //��ƺ��ο� ���ݸ���� �������� �Լ� �����ʿ�
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
			ImpactDirection.Z += R_Angle;
			ImpactDirection = ImpactDirection.GetSafeNormal();

			// Log the impact direction for debugging
			UE_LOG(LogTemp, Log, TEXT("Impact Direction: %s"), *ImpactDirection.ToString());

			// Apply an impulse to the Diavolo character based on the impact direction and AirborneRate
			Diavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * R_Impulse, true);

			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}
		}

	}

}

void AOPMalphite::R_BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
