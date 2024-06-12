// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/LeeSin/OPLeeSin.h"
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"
#include "Sound/SoundCue.h"

// Sets default values
AOPLeeSin::AOPLeeSin()
{
	SonicWave = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Sonic Wave")); // 투사체 움직임 포인터에 동적 할당
	DragonRage = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Dragon's Rage")); // 투사체 움직임 포인터에 동적 할당
}

// Called when the game starts or when spawned
void AOPLeeSin::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPLeeSin::Passive()
{
	Super::Passive();
}

void AOPLeeSin::MeleeAttack()
{
	Super::MeleeAttack();

	if (!GetbMeleeAttack()) return; // 평타 쿨타임 시 return
	if (!GetOPPlayerController()) return; // 플레이어 컨트롤러가 nullptr 시 return

	switch (MeleeAttackComboCount) // 4번의 연결된 평타동작
	{
	case 0:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("1"), MeleeAttackAnimMontage);
		GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
		MeleeAttackComboCount++;
		break;

	case 1:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("2"), MeleeAttackAnimMontage);
		GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimer);
		GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
		MeleeAttackComboCount++;
		break;

	case 2:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("3"), MeleeAttackAnimMontage);
		GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimer);
		GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPLeeSin::ResetMeleeAttackComboCount, 5.f, false);
		MeleeAttackComboCount++;
		break;

	case 3:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("4"), MeleeAttackAnimMontage);
		MeleeAttackComboCount = 0;
		break;

	default:
		;
	}

	// 평타 쿨타임 설정을 위한 두 줄
	SetbMeleeAttack_False();
	GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPLeeSin::SetbMeleeAttack_True, GetMeleeAttackCooltime(), false);
}

void AOPLeeSin::Skill_1()
{
	Super::Skill_1();

	if (!GetbSkill_1()) return;
	if (!GetOPPlayerController()) return;
}

void AOPLeeSin::Skill_1_SonicWave()
{
}

void AOPLeeSin::Skill_2()
{
}

void AOPLeeSin::Skill_3()
{
}

void AOPLeeSin::Ult()
{
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

