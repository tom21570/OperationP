// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGravity/CustomGravityActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Champion/OPChampion.h" // OPChampion 캐릭터가 여기에 포함된 헤더파일


ACustomGravityActor::ACustomGravityActor()
{
}

void ACustomGravityActor::BeginPlay()
{
    Super::BeginPlay();
}

void ACustomGravityActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
