// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/OPLeeSin.h"

// Sets default values
AOPLeeSin::AOPLeeSin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOPLeeSin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPLeeSin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOPLeeSin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

