// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Yasuo/OPYasuoWindWall.h"
#include "Champion/OPChampion.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AOPYasuoWindWall::AOPYasuoWindWall()
{
	PrimaryActorTick.bCanEverTick = true;
	// RootCapsule 먼저 초기화
	RootCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCapsule"));
	RootComponent = RootCapsule;

	// RootCapsule의 물리 설정
	RootCapsule->SetSimulatePhysics(true);
	RootCapsule->SetEnableGravity(false);
	RootCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	RootCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	RootCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	// ProjectileMovementComponent 초기화
	OPProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	OPProjectileMovementComponent->SetUpdatedComponent(RootCapsule);
	OPProjectileMovementComponent->ProjectileGravityScale = 0.f; // 중력 효과 없애기
	OPProjectileMovementComponent->bRotationFollowsVelocity = true;
	OPProjectileMovementComponent->bShouldBounce = false;
}

// Called when the game starts or when spawned
void AOPYasuoWindWall::BeginPlay()
{
	Super::BeginPlay();

	if (OPProjectileMovementComponent)
	{
		StartLocation = GetActorLocation();

		// Spawn Niagara effect
		if (ProjectileEffect)
		{
			ProjectileNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ProjectileEffect,
				RootComponent,
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true
			);
		}
	}
}

// Called every frame
void AOPYasuoWindWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceTravelled = FVector::Dist(StartLocation, GetActorLocation());
	if (DistanceTravelled >= DistanceToStop)
	{
		StopProjectile();
	}
}

void AOPYasuoWindWall::StopProjectile()
{
	OPProjectileMovementComponent->StopMovementImmediately();
	// Detach the Niagara component to stop it from updating its position
	if (ProjectileNiagaraComponent)
	{
		ProjectileNiagaraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AOPYasuoWindWall::DestroyProjectile, StopDuration, false);
}

void AOPYasuoWindWall::DestroyProjectile()
{
	Destroy();
}

void AOPYasuoWindWall::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		AOPDiavolo* TestDiavolo = Cast<AOPDiavolo>(OtherActor);
		if (TestDiavolo)
		{
			FVector ImpactDirection = (TestDiavolo->GetActorLocation() - SweepResult.ImpactPoint).GetSafeNormal();
			ImpactDirection.Z += 1.0f;
			ImpactDirection = ImpactDirection.GetSafeNormal();

			TestDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * WindWallForce, true);
			if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
			{
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
			}
		}
		DestroyProjectile();
	}
}

void AOPYasuoWindWall::InitProjectile(FVector Direction, float Speed)
{
	Direction.Z = 0;
	
	// 초기 속도가 올바르게 설정되었는지 확인
	if (OPProjectileMovementComponent)
	{

		OPProjectileMovementComponent->InitialSpeed = Speed;
		OPProjectileMovementComponent->MaxSpeed = Speed;
		OPProjectileMovementComponent->Velocity = Direction * Speed;

	}


	// 캐릭터보다 살짝 앞에서 발사체가 스폰되도록 설정
	SetActorLocation(GetActorLocation() + Direction * 100.f);
}