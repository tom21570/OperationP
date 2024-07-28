// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Volibear/OPVolibear.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/Volibear/OPVolibearLightningbolt.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/OPPlayerController.h"

AOPVolibear::AOPVolibear()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AOPVolibear::BeginPlay()
{
	Super::BeginPlay();
}

void AOPVolibear::Passive()
{
	Super::Passive();
}

void AOPVolibear::MeleeAttack()
{
	Super::MeleeAttack();

	if(!bMeleeAttack) return; // 평타 쿨타임 시 return
	if(OPPlayerController == nullptr) return; // 플레이어 컨트롤러가 nullptr 시 return

	// ECC_Visibility 채널에 대한 반응이 overlap 또는 block인 액터에 hit 했을 시 GetHitResultUnderCursor는 그 액터에 대한 HitResult를 MouseCursorHit에 저장.
	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if(MouseCursorHit.bBlockingHit) // 만약 반응이 block이라면 그 Hit 방향으로 캐릭터를 돌림
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	// 평타 쿨타임 설정을 위한 두 줄
	SetbMeleeAttack_False();
	GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPVolibear::SetbMeleeAttack_True, GetMeleeAttackCooltime(), false);

	// 평타 시전시간 지나면 Trace하고 디아볼로가 Trace되면 피격 사운드 재생
	GetWorldTimerManager().SetTimer(MeleeAttackCastTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		if (MeleeAttackTrace())
		{
			
		};
	}), 0.25f, false);

	if (!ChampionAnimInstance) return; // 애니메이션 인스턴스가 없을 시 return
	if (!MeleeAttackAnimMontage) return; // 평타 애니메이션 몽타주가 없을 시 return

	if (ChampionAnimInstance && MeleeAttackAnimMontage)
	{
		if (bThunderingSmash)
		{
			ChampionAnimInstance->Montage_Play(MeleeAttackAnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection(FName("Thundering Smash"), MeleeAttackAnimMontage);
		}

		else
		{
			switch (MeleeAttackComboCount) // 4번의 연결된 평타동작
			{
			case 0:
				ChampionAnimInstance->Montage_Play(MeleeAttackAnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("1"), MeleeAttackAnimMontage);
				GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimerHandle, this, &AOPVolibear::ResetMeleeAttackComboCount, 5.f, false);
				MeleeAttackComboCount++;
				break;

			case 1:
				ChampionAnimInstance->Montage_Play(MeleeAttackAnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("2"), MeleeAttackAnimMontage);
				GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimerHandle);
				GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimerHandle, this, &AOPVolibear::ResetMeleeAttackComboCount, 5.f, false);
				MeleeAttackComboCount++;
				break;

			case 2:
				ChampionAnimInstance->Montage_Play(MeleeAttackAnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("3"), MeleeAttackAnimMontage);
				GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimerHandle);
				GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimerHandle, this, &AOPVolibear::ResetMeleeAttackComboCount, 5.f, false);
				MeleeAttackComboCount++;
				break;

			case 3:
				ChampionAnimInstance->Montage_Play(MeleeAttackAnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("4"), MeleeAttackAnimMontage);
				MeleeAttackComboCount = 0;
				break;

			default:
				;
			}
		}
	}
}

bool AOPVolibear::MeleeAttackTrace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f, 80.f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * MeleeAttack_Impulse, true);
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}

			return true;
		}
	}

	return false;
}

void AOPVolibear::Skill_1()
{
	Super::Skill_1();

	if (!bSkill_1) return;
	if (OPPlayerController == nullptr) return;

	bThunderingSmash = true;

	
}

void AOPVolibear::Skill_2()
{
	Super::Skill_2();
}

void AOPVolibear::Skill_3()
{
	Super::Skill_3();

	if (!bSkill_3) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	Skill_3_FinalLocation = MouseCursorHit.Location;

	GetWorldTimerManager().SetTimer(LightningboltSpawnTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		Skill_3_Lightningbolt();
	}), 2.f, false);
}

void AOPVolibear::Skill_3_Lightningbolt()
{
	Lightningbolt = GetWorld()->SpawnActor<AOPVolibearLightningbolt>(LightningboltClass, Skill_3_FinalLocation, GetActorRotation());
	Lightningbolt->SetOwner(this);
}

void AOPVolibear::Skill_4()
{
	Super::Skill_4();
}

void AOPVolibear::Ult()
{
	Super::Ult();

	if (!bUlt) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	Ult_FinalLocation = MouseCursorHit.Location;
	FVector UltVector = Ult_FinalLocation - GetActorLocation();
	
	FVector UltVector_XY = UltVector.GetSafeNormal();
	UltVector_XY.Z = 0.f;

	// ProjectileMovementComponent->Velocity = GetActorForwardVector() * 100000.f;
	
	// Launch 방식
	if (bUlt_ActAsProjectile)
	{
		LaunchCharacter(UltVector_XY * Ult_Velocity_XY + GetActorUpVector() * Ult_Velocity_Z, true, true); // 속도
	}

	// Game 방식
	if (bUlt_ActAsGame)
	{
		if (ProjectileMovementComponent == nullptr) return;
		
		const FVector FinalVelocity = GetActorForwardVector() * Ult_Velocity_XY + GetActorUpVector() * Ult_Velocity_Z;;

		ProjectileMovementComponent->Velocity = FinalVelocity;
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
		GetWorldTimerManager().SetTimer(Ult_StopTimerHandle, FTimerDelegate::CreateLambda([&]
		{
			ProjectileMovementComponent->Velocity = FVector::Zero();
			ProjectileMovementComponent->ProjectileGravityScale = 1.f;
		}), 0.1f, false);
	}

	// 포물선 운동 방식
	else if (bUlt_ActAsParabola)
	{
		if (ProjectileMovementComponent == nullptr) return;
		
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, Ult_Velocity_Parabola, GetActorLocation(), Ult_FinalLocation, 1.f, 0.5);
		GetCharacterMovement()->AddImpulse(Ult_Velocity_Parabola, true);
	}

	// Game 방식
	
	// 포물선 운동
	// if (ProjectileMovementComponent)
	// {
	// 	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, Ult_Velocity_Parabola, GetActorLocation(), Ult_FinalLocation, 1.f, 0.5);
	// 	GetCharacterMovement()->AddImpulse(Ult_Velocity_Parabola, true);
	// }

	if (ChampionAnimInstance && Ult_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Ult_AnimMontage, 1.f);
	}
}

void AOPVolibear::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
}
