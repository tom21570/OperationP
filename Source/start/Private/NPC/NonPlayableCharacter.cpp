// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NonPlayableCharacter.h"
#include "Projectile/OPProjectile.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ANonPlayableCharacter::ANonPlayableCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANonPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 2�� �������� ThrowProjectile �Լ��� ȣ���ϵ��� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ANonPlayableCharacter::ThrowProjectile, 2.0f, true);
}

// Called every frame
void ANonPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANonPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANonPlayableCharacter::ThrowProjectile()
{
	if (ProjectileClass)
	{
		FVector Location = GetActorLocation() + GetActorForwardVector() * 100.0f;
		FRotator Rotation = GetActorRotation();

		GetWorld()->SpawnActor<AOPProjectile>(ProjectileClass, Location, Rotation);
	}
}
