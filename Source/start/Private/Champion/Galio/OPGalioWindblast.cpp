// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Galio/OPGalioWindblast.h"
#include "Diavolo/OPDiavolo.h"

void AOPGalioWindblast::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                     SweepResult);

	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		if (AOPGalioWindblast* OtherWindblast = Cast<AOPGalioWindblast>(OtherActor))
		{
			// Destroy();
		}
		else if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(OtherActor))
		{
			Diavolo->PlayDiavoloRandomDeadMontage();
			UE_LOG(LogTemp, Warning, TEXT("Diavolo OVerlapped"));
		}
	}
}
