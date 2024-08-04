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

void AOPVolibear::BasicAttack()
{
	Super::BasicAttack();

	if(!bBasicAttack) return; // 평타 쿨타임 시 return
	if(OPPlayerController == nullptr) return; // 플레이어 컨트롤러가 nullptr 시 return

	// ECC_Visibility 채널에 대한 반응이 overlap 또는 block인 액터에 hit 했을 시 GetHitResultUnderCursor는 그 액터에 대한 HitResult를 MouseCursorHit에 저장.
	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if(MouseCursorHit.bBlockingHit) // 만약 반응이 block이라면 그 Hit 방향으로 캐릭터를 돌림
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	// 평타 쿨타임 설정을 위한 두 줄
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttackCooltimeTimerHandle, this, &AOPVolibear::SetbBasicAttack_True, GetBasicAttackCooltime(), false);

	// 평타 시전시간 지나면 Trace하고 디아볼로가 Trace되면 피격 사운드 재생
	GetWorldTimerManager().SetTimer(BasicAttackCastTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		if (MeleeAttackTrace())
		{
			
		}
	}), 0.25f, false);

	if (!ChampionAnimInstance) return; // 애니메이션 인스턴스가 없을 시 return
	if (!BasicAttackAnimMontage) return; // 평타 애니메이션 몽타주가 없을 시 return

	if (ChampionAnimInstance && BasicAttackAnimMontage)
	{
		if (bThunderingSmash)
		{
			ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection(FName("Thundering Smash"), BasicAttackAnimMontage);
		}

		else
		{
			switch (BasicAttackComboCount) // 2번의 연결된 평타동작
			{
			case 0:
				ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("1"), BasicAttackAnimMontage);
				GetWorldTimerManager().SetTimer(BasicAttackComboCountTimerHandle, this, &AOPVolibear::ResetMeleeAttackComboCount, 5.f, false);
				BasicAttackComboCount++;
				break;

			case 1:
				ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("2"), BasicAttackAnimMontage);
				GetWorldTimerManager().ClearTimer(BasicAttackComboCountTimerHandle);
				GetWorldTimerManager().SetTimer(BasicAttackComboCountTimerHandle, this, &AOPVolibear::ResetMeleeAttackComboCount, 5.f, false);
				BasicAttackComboCount = 0;
				break;
				
			default:
				;
			}
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPVolibear::ResetChampionMovement, 1.05f, false);
    
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttackCooltimeTimerHandle, this, &AOPVolibear::SetbBasicAttack_True, BasicAttackCooldown, false);
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

void AOPVolibear::Q() //번개 강타 Q 볼리베어가 적을 향해 이동할 때 이동 속도가 증가하며 처음으로 기본 공격하는 대상을 기절시키고 피해를 입힙니다.
{
	Super::Q(); 

	if (!bQ) return;
	if (OPPlayerController == nullptr) return;

	bThunderingSmash = true;
}

void AOPVolibear::W() //광란의 상처 W 볼리베어가 적에게 피해를 입혀 적중 시 효과를 적용하고 표식을 남깁니다.표식을 남긴 대상에게 다시 이 스킬을 사용하면 추가 피해를 입히고 체력을 회복합니다.
{
	Super::W();

	if (!bW) return;
	if (OPPlayerController == nullptr) return;

	ChampionAnimInstance->Montage_Play(W_AnimMontage, 1.f);

	if (AOPDiavolo * ReturnedDiavolo = Cast<AOPDiavolo>(Skill_2_Trace()))
	{
		GetWorldTimerManager().SetTimer(Skill_2_SpawnTimerHandle, [this, ReturnedDiavolo]()
		{
				SetbW_True();
				RemoveMarkerOnTarget(ReturnedDiavolo);
		}, 8.0, false);

		if (ReturnedDiavolo->GetbFrenziedMaulOn())
		{
			//추가피해 + 체력 회복
		}
		else
		{
			// 마커생성
			CreateMarkerOnTarget(ReturnedDiavolo);
		}

	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPVolibear::ResetChampionMovement, 1.f, false);

	SetbW_False();
	GetWorldTimerManager().SetTimer(W_CooldownTimerHandle, this, &AOPVolibear::SetbW_True, W_Cooldown, false);
}

AOPDiavolo* AOPVolibear::Skill_2_Trace()
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
			Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * Skill_2_Impulse, true);
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}

			return Diavolo;
		}
	}

	return nullptr;
}

void AOPVolibear::E() //천공 분열E 볼리베어가 지정한 위치에 번개를 소환해 적에게 피해를 입히고 둔화시킵니다.볼리베어가 폭발 반경 안에 있으면 보호막을 얻습니다.
{
	Super::E();

	if (!bE) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	Skill_3_FinalLocation = MouseCursorHit.Location;

	GetWorldTimerManager().SetTimer(LightningboltSpawnTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		Skill_3_Lightningbolt();
	}), 2.f, false);

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage, 1.f);
	}
	
	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPVolibear::ResetChampionMovement, 2.6f, false);

	SetbE_False();
	GetWorldTimerManager().SetTimer(E_CooldownTimerHandle, this, &AOPVolibear::SetbE_True, E_Cooldown, false);
}

void AOPVolibear::Skill_3_Lightningbolt() //
{
	if (LightningboltClass)
	{
		Lightningbolt = GetWorld()->SpawnActor<AOPVolibearLightningbolt>(LightningboltClass, Skill_3_FinalLocation, GetActorRotation());
		Lightningbolt->SetOwner(this);
	}
}

void AOPVolibear::R() //폭풍을 부르는 자 R 볼리베어가 지정한 위치로 도약하여 아래에 있는 적을 둔화시키고 피해를 입히며 추가 체력을 얻습니다.볼리베어가 착지한 곳 근처에 있는 포탑은 일시적으로 비활성화됩니다.
{
	Super::R();

	if (!bR) return;
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

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage, 1.f);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPVolibear::ResetChampionMovement, 2.f, false);

	SetbR_False();
	GetWorldTimerManager().SetTimer(R_CooldownTimerHandle, this, &AOPVolibear::SetbR_True, R_Cooldown, false);
}

void AOPVolibear::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
}

void AOPVolibear::CreateMarkerOnTarget(AOPDiavolo* Target)
{
	if (MarkerMesh && Target)
	{
		Target->SetbFrenziedMaulOn_True();
		MarkerMesh->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		MarkerMesh->SetRelativeLocation(FVector(0, 0, 100));
		MarkerMesh->SetVisibility(true);
	}
}

void AOPVolibear::RemoveMarkerOnTarget(AOPDiavolo* Target)
{
	if (MarkerMesh && Target)
	{
		Target->SetbFrenziedMaulOn_False();
		MarkerMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		MarkerMesh->SetVisibility(false);
	}
}
