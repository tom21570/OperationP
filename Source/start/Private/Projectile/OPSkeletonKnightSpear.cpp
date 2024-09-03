// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/OPSkeletonKnightSpear.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Champion/OPChampion.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AOPSkeletonKnightSpear::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                                     SweepResult);

	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		if (AOPChampion* DamagedChampion = Cast<AOPChampion>(OtherActor))
		{
			if (USkeletalMeshComponent* ChampionMesh = DamagedChampion->GetMesh())
			{
				OPProjectileMovementComponent->StopMovementImmediately();
				OPProjectileMovementComponent->ProjectileGravityScale = 0.f;
				AttachToComponent(ChampionMesh, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}
