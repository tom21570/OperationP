// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/OPProjectile.h"
#include "Champion/OPChampion.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AOPProjectile::AOPProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root Capsule"));
	SetRootComponent(RootCapsule);

	OPProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("OP Projectile Movement Component"));
}

// Called when the game starts or when spawned
void AOPProjectile::BeginPlay()
{
	Super::BeginPlay();

	RootCapsule->OnComponentBeginOverlap.AddDynamic(this, &AOPProjectile::OnDamageCollisionBeginOverlap);
}

void AOPProjectile::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, FString(TEXT("Overlapped")));;
}

// Called every frame
void AOPProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
