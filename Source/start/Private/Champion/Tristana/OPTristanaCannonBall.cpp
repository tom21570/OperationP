// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Tristana/OPTristanaCannonBall.h"
#include "Champion/OPChampion.h"
#include "Animation/AnimInstance.h" // �ִϸ��̼� �ν��Ͻ� ��� ����
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Champion/Malphite/OPMalphite.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AOPTristanaCannonBall::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		AOPDiavolo* TestDiavolo = Cast<AOPDiavolo>(OtherActor);
		if (TestDiavolo)
		{
			FVector ImpactDirection = (TestDiavolo->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			ImpactDirection.Z = 0.f;
			ImpactDirection.Z += BasicAttack_AngleOfFloating;
		
			TestDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * BasicAttack_Force, true);
		
			if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
			{
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
			}
		}
		
		AOPMalphite* TestMalphite = Cast<AOPMalphite>(OtherActor);
		if (TestMalphite)
		{
			if (TestMalphite->GetMalphite_HP() > 0)
			{
				TestMalphite->SetMalphite_HP_Damaged(this->BasicAttack_Force, TestMalphite);
			}
			else
			{
				TestMalphite-> PlayDeadAnimMontage();
			}
			
		}
	}
}

void AOPTristanaCannonBall::Tick(float DeltaSeconds)
{
	// AddActorLocalRotation(FRotator(0.f, 0.f, 90.f));
}


