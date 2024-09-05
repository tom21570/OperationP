// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Taric/OPTaricShield.h"
#include "Components/BoxComponent.h"
#include "Projectile/OPProjectile.h"

UOPTaricShield::UOPTaricShield()
{
	// Frustum_FragilePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Frustum_FragilePoint"));
	// Prism_FragilePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Prism_FragilePoint"));
	// Pyramid_FragilePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Pyramid_FragilePoint"));

	// Frustum_FragileCollision = CreateDefaultSubobject<UBoxComponent>("Frustum_FragileCollision");
	// Prism_FragileCollision = CreateDefaultSubobject<UBoxComponent>("Prism_FragileCollision");
	// Pyramid_FragileCollision = CreateDefaultSubobject<UBoxComponent>("Pyramid_FragileCollision");
	//
	// Frustum_FragileCollision->SetupAttachment(GetAttachmentRootActor())
	
	// Frustum_FragileCollision->SetupAttachment(Frustum_FragilePoint);
	// Prism_FragileCollision->SetupAttachment(Prism_FragilePoint);
	// Pyramid_FragileCollision->SetupAttachment(Pyramid_FragilePoint);
}

void UOPTaricShield::BeginPlay()
{
	Super::BeginPlay();
}

// void UOPTaricShield::OnFragileCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	// if (OtherActor)
// 	// {
// 	// 	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Fragiled");
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Fragiled"))
// 	// 	if (AOPProjectile* Projectile = Cast<AOPProjectile>(OtherActor))
// 	// 	{
// 	// 		SetDamageThreshold(CustomDamageThreshold);
// 	// 	}
// 	// }
// }
