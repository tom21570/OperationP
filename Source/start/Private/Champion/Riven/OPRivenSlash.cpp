// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Riven/OPRivenSlash.h"
#include "Champion/Yasuo/OPYasuoWindWall.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"

AOPRivenSlash::AOPRivenSlash()
{
}

void AOPRivenSlash::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AOPRivenSlash::BeginPlay()
{
	Super::BeginPlay();
}

void AOPRivenSlash::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                     SweepResult);

	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(OtherActor))
		{
			Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * Slash_Strength, true);
			Diavolo->PlayDiavoloRandomDeadMontage();
		}
	}
}
