// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/LeeSin/OPLeeSin.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/LeeSin/OPLeeSinSonicWave.h"
#include "Champion/LeeSin/OPLeeSinDragonsRage.h"
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
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // 투사체 움직임 포인터에 동적 할당
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

	// ECC_Visibility 채널에 대한 반응이 overlap 또는 block인 액터에 hit 했을 시 GetHitResultUnderCursor는 그 액터에 대한 HitResult를 MouseCursorHit에 저장.

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit) // 만약 반응이 block이라면 그 Hit 방향으로 캐릭터를 돌림
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

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

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	check(GetChampionAnimInstance());
	check(GetSkill_1_AnimMontage());



}

void AOPLeeSin::Skill_1_SonicWave()
{

}

void AOPLeeSin::Skill_2()//아군에게 이동해서 보호막
{
	Super::Skill_2();

	if (!GetbSkill_2()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;

}

void AOPLeeSin::Skill_3()
{
}

void AOPLeeSin::Ult()
{
	Super::Ult();

	if (!GetbUlt()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	check(GetChampionAnimInstance());
	check(GetSkill_2_AnimMontage());

	GetChampionAnimInstance()->Montage_Play(GetUlt_AnimMontage(), 1.0f);

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	UE_LOG(LogTemp, Log, TEXT("Skill_Ult_DragonsRage"));
	GetWorldTimerManager().SetTimer(DragonsRageSpawnTimer, this, &AOPLeeSin::Skill_Ult_DragonsRage, 0.25f, false);

	Skill_Ult_DragonsRage();
}

void AOPLeeSin::Skill_Ult_DragonsRage()
{
	if (DragonsRageClass == nullptr) return;
	// 현재 위치와 방향을 가져옵니다.
	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	// ForwardVector에 적절한 거리를 곱하여 정면 조금 앞의 위치를 계산합니다.
	// 여기서 100.0f는 스폰할 거리로, 원하는 대로 조정할 수 있습니다.
	FVector SpawnLocation = CurrentLocation + ForwardVector * 100.0f;

	DragonsRage = GetWorld()->SpawnActor<AOPLeeSinDragonsRage>(DragonsRageClass, SpawnLocation, GetActorRotation());
	DragonsRage->SetOwner(this);
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

