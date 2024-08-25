// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Kennen/OPKennenShuriken.h"

#include "Champion/Kennen/OPKennen.h"
#include "Diavolo/OPDiavolo.h"

AOPKennenShuriken::AOPKennenShuriken()
{
}

void AOPKennenShuriken::BeginPlay()
{
	Super::BeginPlay();
}

void AOPKennenShuriken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.f, 30.f, 0.f));
}

void AOPKennenShuriken::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                     SweepResult);

	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		AOPKennen* Kennen = Cast<AOPKennen>(GetOwner());
		if (AOPDiavolo* OverlappedDiavolo = Cast<AOPDiavolo>(OtherActor))
		{
			switch (ShurikenType)
			{
			case EShurikenType::Shuriken_BasicAttack:
				OverlappedDiavolo->PlayDiavoloRandomDamagedMontage();
				break;
				
			case EShurikenType::Shuriken_Q:
				Kennen->Passive_StormMarkOthers(OverlappedDiavolo);
				if (OverlappedDiavolo->GetStormMarkCount() == 3)
				{
					OverlappedDiavolo->ResetStormMark();
					Kennen->Passive_StunOthers(OverlappedDiavolo);
				}
				
				OverlappedDiavolo->PlayDiavoloRandomDamagedMontage();
				break;
				
			case EShurikenType::Shuriken_W:
				Kennen->Passive_StormMarkOthers(OverlappedDiavolo);
				if (OverlappedDiavolo->GetStormMarkCount() == 3)
				{
					OverlappedDiavolo->ResetStormMark();
					Kennen->Passive_StunOthers(OverlappedDiavolo);
				}
				OverlappedDiavolo->PlayDiavoloRandomDamagedMontage();
				break;
			}

			Destroy();
		}
	}
}
