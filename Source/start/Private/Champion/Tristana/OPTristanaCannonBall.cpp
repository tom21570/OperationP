// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Tristana/OPTristanaCannonBall.h"
#include "Champion/OPChampion.h"
#include "Animation/AnimInstance.h" // 애니메이션 인스턴스 헤더 포함
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Champion/Malphite/OPMalphite.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AOPTristanaCannonBall::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
		SweepResult);

	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		AOPDiavolo* TestDiavolo = Cast<AOPDiavolo>(OtherActor);
		if (TestDiavolo)
		{            // Calculate the direction of the impulse
			FVector ImpactDirection = (TestDiavolo->GetActorLocation() - SweepResult.ImpactPoint).GetSafeNormal();

			// Add an upward component to the impact direction
			ImpactDirection.Z += AngleOfFloating;
			ImpactDirection = ImpactDirection.GetSafeNormal();

			// Log the impact direction for debugging
			UE_LOG(LogTemp, Log, TEXT("Impact Direction: %s"), *ImpactDirection.ToString());

			// Apply an impulse to the Diavolo character based on the impact direction and AirborneRate
			TestDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * ForceOfCannonBall, true);

			if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
			{
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}
		}

		AOPMalphite* TestMalphite = Cast<AOPMalphite>(OtherActor);
		if (TestMalphite)
		{
			TestMalphite->PlayDeadAnimMontage();
		}
	}
}

void AOPTristanaCannonBall::Tick(float DeltaSeconds)
{
}


