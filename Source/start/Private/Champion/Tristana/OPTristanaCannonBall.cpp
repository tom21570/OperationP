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

}

void AOPTristanaCannonBall::Tick(float DeltaSeconds)
{
}


