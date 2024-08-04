// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Tristana/OPTristana.h"
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"
#include "Champion/Tristana/OPTristanaCannonBall.h"
#include "Champion/Tristana/OPTristanaBusterShot.h"
#include "Champion/Tristana/OPTristanaExplosiveCharge.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/SkeletalMeshSocket.h"

AOPTristana::AOPTristana()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // 투사체 움직임 포인터에 동적 할당

	CannonBallSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CannonBall SpawnPoint"));
	CannonBallSpawnPoint->SetupAttachment(GetMesh(), FName(TEXT("CannonSocket")));

	RapidFireNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RapidFire Niagara"));
	RapidFireNiagaraComponent->SetupAttachment(GetMesh(), FName(TEXT("CannonSocket")));
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

void AOPTristana::BeginPlay()
{
	Super::BeginPlay();

	DefaultAttackSpeed = BasicAttackCooltime;

	Skill_2_LongJumpRange = Skill_2_MaxJumpRange;

	if (RapidFireNiagaraComponent)
	{
		RapidFireNiagaraComponent->SetVisibility(false);
	}
}

void AOPTristana::Tick(float DeltaTime)
{
}

void AOPTristana::Passive()
{

}

void AOPTristana::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(CannonBallSpawnTimer, this, &AOPTristana::BasicAttack_CannonBall, 0.25f, false);

	if (ChampionAnimInstance && BasicAttackAnimMontage)
	{
		ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.0f);
	}
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.7f, false);

	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttackCooltimeTimerHandle, this, &AOPTristana::SetbBasicAttack_True, BasicAttackCooltime, false);
	//GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPTristana::SetbMeleeAttack, GetMeleeAttackCooltime(), false); //이부분은 왜 SetTimer가 빨간줄이 쳐질까요
	
	
	// FVector Start = GetActorLocation();
	// FVector TestDiavoloLocation = TestDiavolo->GetActorLocation();
	//
	// float DistanceToTarget = CalculateMinDistanceToActorEdge(Start, TestDiavoloLocation, LongDistanceAttack_Range);
	//
	// FTimerHandle LongDistanceAttack_EndTimer;
	//
	//
	// if (DistanceToTarget <= LongDistanceAttack_Range)
	// {
	// 	// 공격 범위 내에 있으면 공격
	// 	FHitResult HitResult;
	// 	FCollisionQueryParams Params;
	// 	Params.AddIgnoredActor(this);
	//
	// 	GetChampionAnimInstance()->Montage_Play(LongDistanceAttackAnimMontage, 1.f);
	//
	// 	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, TestDiavoloLocation, ECC_Visibility, Params))
	// 	{
	// 		// 데미지 적용
	// 		if (AActor* HitActor = HitResult.GetActor())
	// 		{
	// 			// 임시로 로그 출력
	// 			UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetName());
	// 			//디아볼로 쓰러지는 모션 혹은 공격당하는 모션 추가
	// 		}
	// 	}
	// }
	// else
	// {
	// 	// 공격 범위 밖에 있으면 이동
	// 	FVector Direction = (TestDiavoloLocation - Start).GetSafeNormal();
	// 	FVector MoveLocation = TestDiavoloLocation - Direction * LongDistanceAttack_Range;
	//
	// 	UE_LOG(LogTemp, Warning, TEXT("Moving to: %s"), *MoveLocation.ToString());
	// 	MoveToLocation(MoveLocation);
	//
	// }
		
}

void AOPTristana::BasicAttack_CannonBall()
{
	if (CannonBallClass == nullptr) return;

	FRotator CannonBallRotator = FRotator(BasicAttack_Angle, GetActorRotation().Yaw, GetActorRotation().Roll);
	FVector CannonBallVelocity = BasicAttack_Speed * CannonBallRotator.Vector();
	
	if (CannonBall = GetWorld()->SpawnActor<AOPTristanaCannonBall>(CannonBallClass, CannonBallSpawnPoint->GetComponentLocation(), GetActorRotation()))
	{
		CannonBall->GetOPProjectileMovementComponent()->Velocity = CannonBallVelocity;
		CannonBall->SetOwner(this);
		FVector LaunchDirection = GetActorForwardVector();
	}
}


void AOPTristana::Q()  //빠른 발사 (Rapid Fire) 효과: 일정 시간 동안 트리스타나의 공격 속도가 대폭 증가합니다.
{
	Super::Q();
	//대포에 불붙는 vfx 추가 필요

	bIsRapidFireActive = true;

	BasicAttackCooltime *= RapidFireValue;

	if (RapidFireNiagaraComponent)
	{
		RapidFireNiagaraComponent->SetVisibility(true);
	}
	GetWorldTimerManager().SetTimer(RapidFireTimerHandle, this, &AOPTristana::EndRapidFire, RapidFireDuration, false);

	GetWorldTimerManager().SetTimer(Skill_1_CooltimeTimerHandle, this, &AOPTristana::SetbSkill_1_True, Skill_1_Cooltime, false);
}


void AOPTristana::EndRapidFire()
{
	bIsRapidFireActive = false;

	BasicAttackCooltime = DefaultAttackSpeed;

	if (RapidFireNiagaraComponent)
	{
		RapidFireNiagaraComponent->SetVisibility(false);
	}

	UE_LOG(LogTemp, Warning, TEXT("Rapid Fire ended: Attack speed normalized."));
}

void AOPTristana::W() //로켓 점프 (Rocket Jump) 효과: 트리스타나가 목표 지점으로 로켓을 이용해 뛰어올라, 착지 지점에서 범위 피해를 입히고 적들을 둔화시킵니다. 재사용: 로켓 점프는 적을 처치하거나 어시스트를 기록하면 재사용 대기 시간이 초기화됩니다.
{
	Super::W();

	if (!bSkill_2) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	UE_LOG(LogTemp, Warning, TEXT("cursor Hit"));
	TurnCharacterToCursor(MouseCursorHit);

	// TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	// if (TestDiavolo == nullptr) return;

	// FVector DiavoloLocation = TestDiavolo->GetActorLocation();

	FVector CurrentLocation = GetActorLocation();
	// float Distance = FVector::Dist(CurrentLocation, DiavoloLocation);
	float Distance = FVector::Dist(CurrentLocation, MouseCursorHit.Location);

	if (Distance <= Skill_2_MaxJumpRange)
	{
		Skill_2_FinalLocation = MouseCursorHit.Location;
		FVector Skill_2_Vector = Skill_2_FinalLocation - GetActorLocation();
	
		Skill_2_Vector_XY = Skill_2_Vector.GetSafeNormal();
		Skill_2_Vector_XY.Z = 0.f;
	}

	GetWorldTimerManager().SetTimer(Skill_2_JumpTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		LaunchCharacter(Skill_2_Vector_XY * Skill_2_Velocity_XY + GetActorUpVector() * Skill_2_Velocity_Z, true, true);
	}), 0.25f, false);

	if (ChampionAnimInstance && Skill_2_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Skill_2_AnimMontage, 1.0f);
	}
	// TargetLocation = DiavoloLocation;
	// FVector JumpDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
	// LaunchCharacter(JumpDirection * Skill_2_JumpStrength, true, true);

	// Set a timer to handle landing effects
	GetWorldTimerManager().SetTimer(Skill_2_CooltimeTimerHandle, this, &AOPTristana::OnLanding, 1.0f, false, 1.0f);
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.7f, false);
    
    SetbSkill_2_False();
    GetWorldTimerManager().SetTimer(Skill_2_CooltimeTimerHandle, this, &AOPTristana::SetbSkill_2_True, Skill_2_Cooltime, false);
}

void AOPTristana::OnLanding()
{
	// Handle landing damage and effects
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	// Apply damage to actors within radius 범위 내의 캐릭터들에게 슬로우 이펙트 부여
	//UGameplayStatics::ApplyRadialDamage(this, Skill_2_LandingDamage, GetActorLocation(), Skill_2_LandingRadius, nullptr, IgnoredActors, this, GetController(), true);
	// Apply slow effect to actors within radius (you need to implement the slow effect)
	// 슬로우 이펙트 넣기
	// This can be done via a gameplay ability system or custom logic

	// Clear timer
	GetWorldTimerManager().ClearTimer(Skill_2_CooltimeTimerHandle);
}

void AOPTristana::PlaySkill_2_JumpAnimMontage()
{
}


void AOPTristana::E() //폭발 화약(Explosive Charge) 		효과: 패시브로, 트리스타나가 처치한 적이 폭발하여 주변 적들에게 피해를 입힙니다.활성화 : 트리스타나가 적에게 폭발물 패키지를 설치합니다.시간이 지나거나 트리스타나가 일정 횟수 공격을 가하면 폭발하여 큰 피해를 입힙니다.
{
	Super::E();

	if (!bSkill_3) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	UE_LOG(LogTemp, Warning, TEXT("cursor Hit"));
	TurnCharacterToCursor(MouseCursorHit);

	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;

	FVector DiavoloLocation = TestDiavolo->GetActorLocation();

	FVector CurrentLocation = GetActorLocation();
	float Distance = FVector::Dist(CurrentLocation, DiavoloLocation);

	if (Distance <= Skill_3_MaxThrowRange)
	{
		if (ChampionAnimInstance && Skill_3_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(Skill_3_AnimMontage, 1.0f);
		}
		GetWorldTimerManager().SetTimer(ExplosiveChargeSpawnTimerHandle, FTimerDelegate::CreateLambda([&]
		{
			UseExplosiveCharge(TestDiavolo);
		}), 0.2f, false);
	}
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.7f, false);
        
    SetbSkill_3_False();
    GetWorldTimerManager().SetTimer(Skill_3_CooltimeTimerHandle, this, &AOPTristana::SetbSkill_3_True, Skill_3_Cooltime, false);
}

void AOPTristana::UseExplosiveCharge(AOPDiavolo* Target)
{
	if (ExplosiveChargeClass && Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("UseExplosiveCharge"));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn the explosive charge
		AOPTristanaExplosiveCharge* ExplosiveChargeNow = GetWorld()->SpawnActor<AOPTristanaExplosiveCharge>(ExplosiveChargeClass, Target->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (ExplosiveChargeNow)
		{
			// ExplosiveChargeNow->AttachToComponent(Target->GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);
			ExplosiveChargeNow->InitializeCharge(Target, Skill_3_Damage, Skill_3_ExplosionRadius, Skill_3_TimeToExplode);
			CurrentExplosiveCharge = ExplosiveChargeNow;
		}
	}
}


void AOPTristana::R() //대구경 탄환 (Buster Shot)효과: 트리스타나가 강력한 탄환을 발사하여 대상에게 피해를 입히고, 대상을 멀리 밀어냅니다. 활성화 : 이 스킬은 적 챔피언을 밀어내는 데 효과적이며, 특히 위험한 적을 멀리 밀어내어 위기 상황에서 탈출하거나 적을 분리시키는 데 사용할 수 있습니다.
{
	Super::R();

	if (!bUlt) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(BusterShotClassSpawnTimer, FTimerDelegate::CreateLambda([&]
	{
		Ult_BusterShot();
	}), 0.3f, false);

	SetbUlt_False();
	GetWorldTimerManager().SetTimer(Ult_CooltimeTimerHandle, this, &AOPTristana::SetbUlt_True, GetUlt_Cooltime(), false);

	if (ChampionAnimInstance && Ult_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Ult_AnimMontage, 1.0f);
	}
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.9f, false);
        
    SetbUlt_False();
    GetWorldTimerManager().SetTimer(Ult_CooltimeTimerHandle, this, &AOPTristana::SetbUlt_True, Ult_Cooltime, false);
}

void AOPTristana::Ult_BusterShot()
{
	UE_LOG(LogTemp, Log, TEXT("Ult_BusterShot"));

	if (BusterShotClass == nullptr) return;

	FRotator BusterShotRotator = FRotator(Ult_Angle, GetActorRotation().Yaw, GetActorRotation().Roll);
	FVector BusterShotVelocity = Ult_Speed * BusterShotRotator.Vector();

	if (BusterShot = GetWorld()->SpawnActor<AOPTristanaBusterShot>(BusterShotClass, CannonBallSpawnPoint->GetComponentLocation(), GetActorRotation()))
	{
		BusterShot->GetOPProjectileMovementComponent()->Velocity = BusterShotVelocity;
		BusterShot->SetOwner(this);
		LaunchCharacter(-BusterShotRotator.Vector() * Ult_Rebound, true, true);
	}
}

/* 범위와 이동 공격은 나중에 구현
// 공격 범위 끝의 모든 점과 대상 위치의 최단 거리를 계산
float AOPTristana::CalculateMinDistanceToActorEdge(FVector ActorLocation, FVector TargetLocation, float Radius)
{
	// 공격 범위 끝의 모든 점과 대상 위치의 최단 거리를 계산
	FVector Direction = (TargetLocation - ActorLocation).GetSafeNormal();
	FVector EdgePoint = ActorLocation + Direction * Radius;

	return FVector::Dist(EdgePoint, TargetLocation);
}

void AOPTristana::MoveToLocation(FVector TargetLocation)
{
	FVector Start = GetActorLocation();
	FVector Direction = (TargetLocation - Start).GetSafeNormal();
	MoveTargetLocation = TargetLocation - Direction * LongDistanceAttack_Range;
	bIsMoving = true;
}
*/




