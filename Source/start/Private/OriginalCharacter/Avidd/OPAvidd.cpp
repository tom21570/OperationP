// Fill out your copyright notice in the Description page of Project Settings.


#include "OriginalCharacter/Avidd/OPAvidd.h"

#include "Animation/OPAnimInstance.h"
#include "Champion/Pyke/OPPykeHarpoon.h"
#include "Player/OPPlayerController.h"

AOPAvidd::AOPAvidd()
{
	HeadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMeshComponent->SetupAttachment(GetMesh(), TEXT("head"));

	// Pyke
	PykeHarpoonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PykeHarpoon"));
	PykeHarpoonMeshComponent->SetupAttachment(GetMesh(), TEXT("WeaponSocket_Pyke"));

	PykeHarpoonSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PythonSpawnPoint"));
	PykeHarpoonSpawnPoint->SetupAttachment(GetMesh(), TEXT("WeaponSocket_Pyke"));

	//Nasus
	NasusWeaponComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NasusWeapon"));
	NasusWeaponComponent->SetupAttachment(GetMesh(), TEXT("WeaponSocket_Nasus"));
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

	switch (AviddType)
	{
	case EAviddType::EAT_Pyke:
		Pyke_Q();
		break;

	case EAviddType::EAT_Nasus:
		Nasus_Q();
		break;

	case EAviddType::EAT_Amumu:
		Amumu_W();
		break;

	default:
		break;
	}
}

void AOPAvidd::Pyke_ThrowHarpoon()
{
	if (PykeHarpoonClass == nullptr) return;
	PykeHarpoonStorage = GetWorld()->SpawnActor<AOPPykeHarpoon>(PykeHarpoonClass, PykeHarpoonSpawnPoint->GetComponentLocation(), PykeHarpoonSpawnRotation);
	PykeHarpoonStorage->SetOwner(this);
	PykeHarpoonMeshComponent->SetHiddenInGame(true);
}

void AOPAvidd::Pyke_Q()
{
	if (OCPlayerController == nullptr) return;
	OCPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	TurnCharacterToCursor(MouseCursorHit);

	PykeHarpoonSpawnRotation = (MouseCursorHit.Location - PykeHarpoonSpawnPoint->GetComponentLocation()).Rotation();

	GetWorldTimerManager().SetTimer(PykeHarpoonThrowTimer, this, &AOPAvidd::Pyke_ThrowHarpoon, 0.55f, false);
	if (OCAnimInstance && Pyke_Q_AnimMontage)
	{
		OCAnimInstance->Montage_Play(Pyke_Q_AnimMontage);
	}
	GetWorldTimerManager().SetTimer(PykeHarpoonVisibleTimer, FTimerDelegate::CreateLambda([&]
	{
		PykeHarpoonMeshComponent->SetHiddenInGame(false);
	}), 5.f, false);
}

void AOPAvidd::Nasus_Q()
{
	if (OCPlayerController == nullptr) return;
	OCPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	TurnCharacterToCursor(MouseCursorHit);

	if (OCAnimInstance && Nasus_Q_AnimMontage)
	{
		OCAnimInstance->Montage_Play(Nasus_Q_AnimMontage);
	}
}

void AOPAvidd::Amumu_W()
{
	if (OCAnimInstance && Amumu_W_AnimMontage)
	{
		OCAnimInstance->Montage_Play(Amumu_W_AnimMontage);
	}
}
