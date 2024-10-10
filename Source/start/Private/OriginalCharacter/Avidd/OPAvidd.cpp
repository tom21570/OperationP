// Fill out your copyright notice in the Description page of Project Settings.


#include "OriginalCharacter/Avidd/OPAvidd.h"

#include "Animation/OPAnimInstance.h"
#include "Champion/Pyke/OPPykeHarpoon.h"
#include "Player/OPPlayerController.h"

AOPAvidd::AOPAvidd()
{
	HeadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMeshComponent->SetupAttachment(GetMesh(), TEXT("head"));

	PykeHarpoonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PykeHarpoon"));
	PykeHarpoonMeshComponent->SetupAttachment(GetMesh(), TEXT("WeaponSocket_R"));

	PykeHarpoonSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PythonSpawnPoint"));
	PykeHarpoonSpawnPoint->SetupAttachment(GetMesh(), TEXT("WeaponSocket_R"));
}

void AOPAvidd::BeginPlay()
{
	Super::BeginPlay();
}

void AOPAvidd::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AOPAvidd::Skill()
{
	Super::Skill();

	if (OCPlayerController == nullptr) return;
	OCPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	TurnCharacterToCursor(MouseCursorHit);

	PykeHarpoonSpawnRotation = (MouseCursorHit.Location - PykeHarpoonSpawnPoint->GetComponentLocation()).Rotation();

	GetWorldTimerManager().SetTimer(PykeHarpoonThrowTimer, this, &AOPAvidd::Pyke_Q, 0.55f, false);
	if (OCAnimInstance && Skill_AnimMontage)
	{
		OCAnimInstance->Montage_Play(Skill_AnimMontage);
	}
	GetWorldTimerManager().SetTimer(PykeHarpoonVisibleTimer, FTimerDelegate::CreateLambda([&]
	{
		PykeHarpoonMeshComponent->SetHiddenInGame(false);
	}), 5.f, false);
}

void AOPAvidd::Pyke_Q()
{
	if (PykeHarpoonClass == nullptr) return;
	PykeHarpoonStorage = GetWorld()->SpawnActor<AOPPykeHarpoon>(PykeHarpoonClass, PykeHarpoonSpawnPoint->GetComponentLocation(), PykeHarpoonSpawnRotation);
	PykeHarpoonStorage->SetOwner(this);
	PykeHarpoonMeshComponent->SetHiddenInGame(true);
}
