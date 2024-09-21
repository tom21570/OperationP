// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Galio/OPGalioTornado.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "start/start.h"

class AOPDiavolo;

void AOPGalioTornado::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(Q_TornadoTimerHandle, FTimerDelegate::CreateLambda([&]
    {
    	Destroy();
    }), Q_TornadoMaintainTime, false);
}

void AOPGalioTornado::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;


	UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), 3000.f, 10000.f,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false,
		ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			HitDiavolo->GetCharacterMovement()->AddImpulse(GetActorUpVector() * Q_TornadoStrength);
			HitDiavolo->SetbIsDamagedTrue();
			HitDiavolo->PlayDiavoloRandomDeadMontage();
		}
	}
}
