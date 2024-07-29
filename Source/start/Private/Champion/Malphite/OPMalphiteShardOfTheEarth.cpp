// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Malphite/OPMalphiteShardOfTheEarth.h"
#include "Champion/OPChampion.h"
#include "Animation/AnimInstance.h" // �ִϸ��̼� �ν��Ͻ� ��� ����
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Champion/Malphite/OPMalphite.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AOPMalphiteShardOfTheEarth::OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnDamageCollisionBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
		SweepResult);
	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		AOPDiavolo* TestDiavolo = Cast<AOPDiavolo>(OtherActor);
		AOPMalphite* MalphiteOwner = Cast<AOPMalphite>(GetOwner());
		if (TestDiavolo)
		{
			// ĳ���� Ŭ�������� ȿ���� �����ϴ� �Լ� ȣ��
			MalphiteOwner->ApplySkill_1_Effect(MalphiteOwner, TestDiavolo);
		}
		//������Ʈ ���� �°� ��ƺ��ΰ� ���ư��ߵǸ� ��ƺ��� ��� �ڵ带 �־������
	}
}

void AOPMalphiteShardOfTheEarth::Tick(float DeltaSeconds)
{
	AddActorLocalRotation(FRotator(30.f, 0.f, 0.f));
}
