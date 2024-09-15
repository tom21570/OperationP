// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Yasuo/OPYasuoWindWall.h"
#include "Projectile/OPProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

AOPYasuoWindWall::AOPYasuoWindWall()
{
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(ProceduralMesh);
	
	// UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(Mesh, 0, ProceduralMesh, true);
}

// Called when the game starts or when spawned
void AOPYasuoWindWall::BeginPlay()
{
	Super::BeginPlay();

	OPProjectileMovementComponent->Velocity = InitialSpeed * GetActorForwardVector();
	// GetWorldTimerManager().SetTimer(DestroyTimer, this, &AOPYasuoWindWall::DestroyProjectile, 4.f, false);
	GetWorldTimerManager().SetTimer(SpeedChangeTimer, this, &AOPYasuoWindWall::ChangeSpeed, 0.25f, false);
}


// Called every frame
void AOPYasuoWindWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOPYasuoWindWall::StopProjectile()
{
}

void AOPYasuoWindWall::DestroyProjectile()
{
	Destroy();
}

void AOPYasuoWindWall::ChangeSpeed()
{
	OPProjectileMovementComponent->Velocity = FinalSpeed * GetActorForwardVector();
}

void AOPYasuoWindWall::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		AOPProjectile* OtherProjectile = Cast<AOPProjectile>(OtherActor);
		if (OtherProjectile)
		{
			FVector ImpactDirection = (OtherProjectile->GetActorLocation() - SweepResult.ImpactPoint).GetSafeNormal();
			ImpactDirection.Z += 1.0f;
			ImpactDirection = ImpactDirection.GetSafeNormal();


			UProjectileMovementComponent* ProjectileMovement = OtherProjectile->FindComponentByClass<UProjectileMovementComponent>();
			if (ProjectileMovement)
			{
				ProjectileMovement->Velocity = ImpactDirection * WindWallForce;
			}
		}
	}
}

void AOPYasuoWindWall::SliceMesh(const FVector& SliceLocation)
{
	// GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "Slice");
	// UProceduralMeshComponent* OtherMesh;
	
	// UKismetProceduralMeshLibrary::SliceProceduralMesh(ProceduralMesh, SliceLocation,
	// 	GetActorUpVector() * 1000000.f, true, OtherMesh, EProcMeshSliceCapOption::CreateNewSectionForCap, Mesh->GetMaterial(0));

	ProceduralMesh->SetSimulatePhysics(true);
	ProceduralMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
