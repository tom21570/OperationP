// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Kennen/OPKennen.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/Kennen/OPKennenShuriken.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"

AOPKennen::AOPKennen()
{
	ShurikenSpawnPoint = CreateDefaultSubobject<USceneComponent>("ShurikenSpawnPoint");
	ShurikenSpawnPoint->SetupAttachment(GetMesh(), "ShurikenSocket");
	
	E_ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("E ParticleSystem Component");
	E_ParticleSystemComponent->SetupAttachment(GetRootComponent());

	E_Collision = CreateDefaultSubobject<USphereComponent>("E Collision");
	E_Collision->SetupAttachment(GetRootComponent());
}

void AOPKennen::BeginPlay()
{
	Super::BeginPlay();

	E_ParticleSystemComponent->SetHiddenInGame(true);
}

void AOPKennen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOPKennen::Passive_StormMarkOthers(AOPChampion* Enemy)
{
	Enemy->IncreaseStormMark();
}

void AOPKennen::Passive_StunOthers(AOPChampion* Enemy)
{
	Enemy->ResetStormMark();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Passive_Stun_NiagaraSystem, Enemy->GetActorLocation());
}

void AOPKennen::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;
	if (OPPlayerController == nullptr) return;
	if (KennenShuriken_BasicAttack_Class == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	if (W_ReinforcedAttack_Stack == 4)
	{
		GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			W_ReinforcedAttack_Stack = 0;;
			KennenShurikenStorage = GetWorld()->SpawnActor<AOPKennenShuriken>(KennenShuriken_W_Class,
				ShurikenSpawnPoint->GetComponentLocation(), GetActorRotation());
			KennenShurikenStorage->SetOwner(this);
			KennenShurikenStorage->SetShurikenType(EShurikenType::Shuriken_BasicAttack);
		}), 0.4f, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			W_ReinforcedAttack_Stack++;
			KennenShurikenStorage = GetWorld()->SpawnActor<AOPKennenShuriken>(KennenShuriken_BasicAttack_Class,
				ShurikenSpawnPoint->GetComponentLocation(), GetActorRotation());
			KennenShurikenStorage->SetOwner(this);
			KennenShurikenStorage->SetShurikenType(EShurikenType::Shuriken_W);
		}), 0.4f, false);
	}
	
	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage);
		int32 RandomValue = FMath::RandRange(0, 2);
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
		default:
			break;
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPKennen::ResetChampionMovement, 1.05f, false);
	bBasicAttack = false;
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPKennen::SetbBasicAttack_True, BasicAttack_Cooldown, false);
}

void AOPKennen::Q()
{
	Super::Q();

	if (!bQ) return;
	if (OPPlayerController == nullptr) return;
	if (KennenShuriken_Q_Class == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(Q_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		KennenShurikenStorage = GetWorld()->SpawnActor<AOPKennenShuriken>(KennenShuriken_Q_Class,
				ShurikenSpawnPoint->GetComponentLocation(), GetActorRotation());
		KennenShurikenStorage->SetOwner(this);
		KennenShurikenStorage->SetShurikenType(EShurikenType::Shuriken_Q);
	}), 0.4f, false);
	
	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage);
		int32 RandomValue = FMath::RandRange(0, 2);
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
		default:
			break;
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPKennen::ResetChampionMovement, 1.05f, false);
	bBasicAttack = false;
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPKennen::SetbBasicAttack_True, BasicAttack_Cooldown, false);
}

void AOPKennen::W()
{
	Super::W();

	if (!bW) return;
	
	if (ChampionAnimInstance && W_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(W_AnimMontage);
	}

	GetWorldTimerManager().SetTimer(W_Cast_TimerHandle, this, &AOPKennen::W_Trace, 0.25f, false);

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPKennen::ResetChampionMovement, 1.05f, false);
	bBasicAttack = false;
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPKennen::SetbBasicAttack_True, BasicAttack_Cooldown, false);
}

void AOPKennen::W_Trace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), W_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo->GetStormMarkCount() > 0)
			{
				HitDiavolo->PlayDiavoloRandomDeadMontage();
				Passive_StormMarkOthers(HitDiavolo);
				if (HitDiavolo->GetStormMarkCount() == 3)
				{
					HitDiavolo->ResetStormMark();
					Passive_StunOthers(HitDiavolo);
				}
			}
		}
	}
}

void AOPKennen::E()
{
	Super::E();

	if (!bE) return;
	
	bE_IsRushing = true;
	WalkSpeed = E_WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	E_Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	E_Collision->OnComponentBeginOverlap.AddDynamic(this, &AOPKennen::E_OnOverlap);
	
	if (E_ParticleSystemComponent)
	{
		E_ParticleSystemComponent->SetHiddenInGame(false);
		GetWorldTimerManager().SetTimer(E_Maintain_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			bE_IsRushing = false;
			WalkSpeed = DefaultWalkSpeed;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
			E_Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			E_Collision->OnComponentBeginOverlap.RemoveDynamic(this, &AOPKennen::E_OnOverlap);
			E_ParticleSystemComponent->SetHiddenInGame(true);
		}), E_MaintainTime, false);
	}
}

void AOPKennen::E_OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapped Kennen E"));
			Diavolo->PlayDiavoloRandomDeadMontage();
			Passive_StormMarkOthers(Diavolo);
			if (Diavolo->GetStormMarkCount() == 3)
			{
				Diavolo->ResetStormMark();
				Passive_StunOthers(Diavolo);
			}
		}
	}
}

void AOPKennen::R()
{
	Super::R();

	if (!bR) return;

	R_Trace();
	GetWorldTimerManager().SetTimer(R_TraceLoop_TimerHandle, this, &AOPKennen::R_Trace, 0.5f, true);
	GetWorldTimerManager().SetTimer(R_TraceStop_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		GetWorldTimerManager().ClearTimer(R_TraceLoop_TimerHandle);
	}), 3.f, false);

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage);
	}

	if (R_NiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(R_NiagaraSystem, GetRootComponent(), NAME_None,
		GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
	}

	bR = false;
	GetWorldTimerManager().SetTimer(R_Cooldown_TimerHandle, this, &AOPKennen::SetbR_True, R_Cooldown, true);
}

void AOPKennen::R_Trace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), R_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			HitDiavolo->IncreaseStormMark();
			if (HitDiavolo->GetStormMarkCount() == 3)
			{
				Passive_StunOthers(HitDiavolo);
			}
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), R_Hit_NiagaraSystem, HitDiavolo->GetActorLocation() + FVector(0.f, 0.f, 150.f), FRotator::ZeroRotator, FVector(1), true);
			HitDiavolo->SetbIsDamagedTrue();
			HitDiavolo->PlayDiavoloRandomDeadMontage();
		}
	}
}
