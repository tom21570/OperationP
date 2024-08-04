// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Malphite/OPMalphiteShardOfTheEarth.h"
#include "Champion/OPChampion.h"
#include "Animation/AnimInstance.h" // �ִϸ��̼� �ν��Ͻ� ��� ����
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Champion/Malphite/OPMalphite.h"
#include "Components/BoxComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AOPMalphiteShardOfTheEarth::AOPMalphiteShardOfTheEarth()
{
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollision"));
	DamageCollision->SetupAttachment(GetRootComponent());
}

void AOPMalphiteShardOfTheEarth::BeginPlay()
{
	Super::BeginPlay();

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AOPMalphiteShardOfTheEarth::OnDamageCollisionBeginOverlap);
}

void AOPMalphiteShardOfTheEarth::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		AOPDiavolo* TestDiavolo = Cast<AOPDiavolo>(OtherActor);
		if (TestDiavolo)
		{
			FVector ImpactDirection = (TestDiavolo->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			ImpactDirection.Z = 0.f;
			ImpactDirection.Z = Q_Angle;
			TestDiavolo->SetbIsDamagedTrue();
			TestDiavolo->PlayDiavoloRandomDeadMontage();
			TestDiavolo->LaunchCharacter(ImpactDirection * Q_Impulse, true, true);
			// HitDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * R_Impulse, true);
		}
	}
}

void AOPMalphiteShardOfTheEarth::Tick(float DeltaSeconds)
{
	AddActorLocalRotation(FRotator(30.f, 0.f, 0.f));
}
