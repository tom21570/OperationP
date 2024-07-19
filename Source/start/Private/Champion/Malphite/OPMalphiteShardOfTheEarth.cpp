// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Malphite/OPMalphiteShardOfTheEarth.h"
#include "Champion/OPChampion.h"
#include "Animation/AnimInstance.h" // 애니메이션 인스턴스 헤더 포함
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
			// 캐릭터 클래스에서 효과를 적용하는 함수 호출
			MalphiteOwner->ApplySkill_1_Effect(MalphiteOwner, TestDiavolo);
		}
		//말파이트 돌에 맞고 디아볼로가 날아가야되면 디아볼로 충격 코드를 넣어줘야함
		Destroy();
	}
}

void AOPMalphiteShardOfTheEarth::Tick(float DeltaSeconds)
{
	AddActorLocalRotation(FRotator(30.f, 0.f, 0.f));
}
