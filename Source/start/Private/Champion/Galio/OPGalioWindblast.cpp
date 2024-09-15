// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Galio/OPGalioWindblast.h"

#include "Champion/Galio/OPGalioTornado.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "start/start.h"

void AOPGalioWindblast::BeginPlay()
{
	Super::BeginPlay();
}

void AOPGalioWindblast::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	WindTrace();
}

void AOPGalioWindblast::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                     SweepResult);

	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		if (AOPGalioWindblast* OtherWindblast = Cast<AOPGalioWindblast>(OtherActor))
		{
			FVector NiagaraSpawnLocation = (GetActorLocation() + OtherWindblast->GetActorLocation()) / 2;
			NiagaraSpawnLocation.Z -= 2000.f;
			TornadoSpawnLocation = (GetActorLocation() + OtherWindblast->GetActorLocation()) / 2;
			TornadoSpawnLocation.Z += 2000.f;
			SpawnTornado(TornadoSpawnLocation);
			if (Q_TornadoParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Q_TornadoParticleSystem, NiagaraSpawnLocation, FRotator::ZeroRotator, Q_Tornado_Size);
			}
			Destroy();
		}
		
		else if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(OtherActor))
		{
			Diavolo->GetCharacterMovement()->AddImpulse(Diavolo->GetActorUpVector() * Q_WindStrength, true);
			Diavolo->PlayDiavoloRandomDeadMontage();
			UE_LOG(LogTemp, Warning, TEXT("Diavolo OVerlapped"));
		}
	}
}

void AOPGalioWindblast::WindTrace() const
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), 400.f, 1500.f,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			HitDiavolo->GetCharacterMovement()->AddImpulse(GetActorUpVector() * Q_WindStrength);
			HitDiavolo->SetbIsDamagedTrue();
			HitDiavolo->PlayDiavoloRandomDeadMontage();
		}
	}
}

void AOPGalioWindblast::SpawnTornado(const FVector& Location) const
{
	GetWorld()->SpawnActor<AOPGalioTornado>(Q_TornadoClass, Location, FRotator::ZeroRotator);
}
