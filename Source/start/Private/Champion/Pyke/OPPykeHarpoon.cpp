// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Pyke/OPPykeHarpoon.h"
#include "CableComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "start/start.h"

AOPPykeHarpoon::AOPPykeHarpoon()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
	Rope = CreateDefaultSubobject<UCableComponent>("Rope");
	Rope->SetupAttachment(Mesh);
}

void AOPPykeHarpoon::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AOPPykeHarpoon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsGrabbedCharacter)
	{
		TraceForCharacter();
	}
}

void AOPPykeHarpoon::TraceForCharacter()
{
	if (OwnerCharacter == nullptr) return;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation(), 20.f,
		UEngineTypes::ConvertToTraceType(ECC_Combat), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (ACharacter* GrabbedCharacter = Cast<ACharacter>(HitResult.GetActor()))
	{
		GrabbedCharacter->GetMesh()->SetSimulatePhysics(true);
		// GrabbedCharacter->GetCharacterMovement()->AddImpulse(-OwnerCharacter->GetActorForwardVector() * GrabStrength_XY +
		// OwnerCharacter->GetActorUpVector() * GrabStrength_Z, true);
		GrabbedCharacter->GetMesh()->AddImpulseToAllBodiesBelow(-OwnerCharacter->GetActorForwardVector() * GrabStrength_XY +
			OwnerCharacter->GetActorUpVector() * GrabStrength_Z);
		
		Mesh->SetSimulatePhysics(true);
		Mesh->AddImpulse(-OwnerCharacter->GetActorForwardVector() * GrabStrength_XY * 10.f +
			OwnerCharacter->GetActorUpVector() * GrabStrength_Z * 10.f);
		
		IsGrabbedCharacter = true;
	}
}
