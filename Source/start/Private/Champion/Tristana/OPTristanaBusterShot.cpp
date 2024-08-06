// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Tristana/OPTristanaBusterShot.h"
#include "Champion/OPChampion.h"
#include "Champion/Malphite/OPMalphite.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AOPTristanaBusterShot::BeginPlay()
{
	Super::BeginPlay();
}

void AOPTristanaBusterShot::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		AOPDiavolo* TestDiavolo = Cast<AOPDiavolo>(OtherActor);
		AOPMalphite* TestMalphite = Cast<AOPMalphite>(OtherActor);
		
		if (TestDiavolo)
		{            
			FVector ImpactDirection = (TestDiavolo->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			ImpactDirection.Z = 0.f;
			ImpactDirection.Z += R_AngleOfFloating;
		
			TestDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * R_Strength, true);
		
			if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
			{
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
			}
		}

		if (TestMalphite)
		{
			TestMalphite->PlayDeadAnimMontage();
		}
	}
}
