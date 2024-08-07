// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Tristana/OPTristana.h"

#include "NiagaraFunctionLibrary.h"
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
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"

AOPTristana::AOPTristana()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // 투사체 움직임 포인터에 동적 할당

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile SpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(GetMesh(), FName(TEXT("CannonSocket")));

	Q_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RapidFire Niagara"));
	Q_NiagaraComponent->SetupAttachment(GetMesh(), FName(TEXT("CannonSocket")));

	W_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("W Collision"));
	W_Collision->SetupAttachment(GetRootComponent());
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

void AOPTristana::BeginPlay()
{
	Super::BeginPlay();

	BasicAttack_DefaultAttackSpeed = BasicAttackCooldown;

	if (Q_NiagaraComponent)
	{
		Q_NiagaraComponent->SetVisibility(false);
	}
}

void AOPTristana::Tick(float DeltaTime)
{
	if (bIsWJumping)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			W_OnLanding();
			W_Collision->OnComponentBeginOverlap.RemoveDynamic(this, &AOPTristana::OnOverlappingDiavolo);
			UE_LOG(LogTemp, Warning, TEXT("Falling End"));
			bIsWJumping = false;
		}
	}
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

	GetWorldTimerManager().SetTimer(BasicAttack_CannonBallSpawnTimerHandle, this, &AOPTristana::BasicAttack_CannonBall, 0.25f, false);

	if (ChampionAnimInstance && BasicAttackAnimMontage)
	{
		ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.0f);
	}
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.7f, false);

	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttackCooltimeTimerHandle, this, &AOPTristana::SetbBasicAttack_True, BasicAttackCooldown, false);		
}

void AOPTristana::BasicAttack_CannonBall()
{
	if (BasicAttack_CannonBallClass == nullptr) return;

	FRotator CannonBallRotator = FRotator(BasicAttack_Angle, GetActorRotation().Yaw, GetActorRotation().Roll);
	FVector CannonBallVelocity = BasicAttack_Speed * CannonBallRotator.Vector();
	
	if (BasicAttack_CannonBallStorage = GetWorld()->SpawnActor<AOPTristanaCannonBall>(BasicAttack_CannonBallClass, ProjectileSpawnPoint->GetComponentLocation(), CannonBallRotator))
	{
		BasicAttack_CannonBallStorage->GetOPProjectileMovementComponent()->Velocity = CannonBallVelocity;
		BasicAttack_CannonBallStorage->SetOwner(this);
	}
}

void AOPTristana::Q()  //빠른 발사 (Rapid Fire) 효과: 일정 시간 동안 트리스타나의 공격 속도가 대폭 증가합니다.
{
	Super::Q();

	bIsQActive = true;

	BasicAttackCooldown *= Q_RapidFireValue;

	if (Q_NiagaraComponent)
	{
		Q_NiagaraComponent->SetVisibility(true);
	}
	GetWorldTimerManager().SetTimer(Q_TimerHandle, this, &AOPTristana::Q_EndRapidFire, Q_Duration, false);

	GetWorldTimerManager().SetTimer(Q_CooldownTimerHandle, this, &AOPTristana::SetbQ_True, Q_Cooldown, false);
}

void AOPTristana::Q_EndRapidFire()
{
	bIsQActive = false;

	BasicAttackCooldown = BasicAttack_DefaultAttackSpeed;

	if (Q_NiagaraComponent)
	{
		Q_NiagaraComponent->SetVisibility(false);
	}

	UE_LOG(LogTemp, Warning, TEXT("Rapid Fire ended: Attack speed normalized."));
}

void AOPTristana::W() //로켓 점프 (Rocket Jump) 효과: 트리스타나가 목표 지점으로 로켓을 이용해 뛰어올라, 착지 지점에서 범위 피해를 입히고 적들을 둔화시킵니다. 재사용: 로켓 점프는 적을 처치하거나 어시스트를 기록하면 재사용 대기 시간이 초기화됩니다.
{
	Super::W();

	if (!bW) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);
	
	GetWorldTimerManager().SetTimer(W_JumpTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		bIsWJumping = true;
		LaunchCharacter(GetActorForwardVector() * W_Speed_XY + GetActorUpVector() * W_Speed_Z, true, true);
		W_Collision->OnComponentBeginOverlap.AddDynamic(this, &AOPTristana::OnOverlappingDiavolo);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), W_ParticleSystem_JumpStart, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 18.f)
			, FRotator::ZeroRotator);
	}), 0.25f, false);

	if (ChampionAnimInstance && W_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(W_AnimMontage, 1.0f);
	}
	
	GetWorldTimerManager().SetTimer(W_CooldownTimerHandle, this, &AOPTristana::W_OnLanding, 1.0f, false, 1.0f);
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.7f, false);
    
    SetbW_False();
    GetWorldTimerManager().SetTimer(W_CooldownTimerHandle, this, &AOPTristana::SetbW_True, W_Cooldown, false);
}

void AOPTristana::W_OnLanding()
{	
	// TArray<FHitResult> HitResults;
	// TArray<AActor*> ActorsToIgnore;
	// ActorsToIgnore.Add(this);
	//
	// UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), W_LandingRadius,
	// 	UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	//
	// for (auto& HitActor : HitResults)
	// {
	// 	if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
	// 	{
	// 		if (HitDiavolo)
	// 		{
	// 			FRotator ExplosionRotation = (HitDiavolo->GetActorLocation() - GetActorLocation()).Rotation();
	// 			FRotator FinalRotation = FRotator(W_LandingStrengthAngle, ExplosionRotation.Yaw, ExplosionRotation.Roll);
	// 			HitDiavolo->GetCharacterMovement()->AddRadialImpulse(GetActorLocation(), W_LandingRadius, W_LandingStrength, RIF_Linear, true);
	// 			HitDiavolo->GetCharacterMovement()->AddImpulse(FinalRotation.Vector() * W_LandingStrength, true);
	// 			HitDiavolo->SetbIsDamagedTrue();
	// 			HitDiavolo->PlayDiavoloRandomDeadMontage();
	// 		}
	// 	}
	// }

	FVector NiagaraSpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 18.f);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), W_NiagaraComponent_Landing, NiagaraSpawnLocation);
}

void AOPTristana::W_Play_JumpAnimMontage()
{
}

void AOPTristana::OnOverlappingDiavolo(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(OtherActor))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			const FVector NiagaraSpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 18.f);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), W_NiagaraComponent_Landing, NiagaraSpawnLocation);
		}
	}
}

void AOPTristana::E() //폭발 화약(Explosive Charge) 		효과: 패시브로, 트리스타나가 처치한 적이 폭발하여 주변 적들에게 피해를 입힙니다.활성화 : 트리스타나가 적에게 폭발물 패키지를 설치합니다.시간이 지나거나 트리스타나가 일정 횟수 공격을 가하면 폭발하여 큰 피해를 입힙니다.
{
	Super::E();

	if (!bE) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;
	
	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage, 1.0f);
	}
	
	GetWorldTimerManager().SetTimer(E_SpawnTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		E_UseExplosiveCharge(TestDiavolo);
	}), 0.2f, false);
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.7f, false);
        
    SetbE_False();
    GetWorldTimerManager().SetTimer(E_CooldownTimerHandle, this, &AOPTristana::SetbE_True, E_Cooldown, false);
}

void AOPTristana::E_UseExplosiveCharge(AOPDiavolo* Target)
{
	if (E_ExplosiveChargeClass && Target)
	{
		FRotator ExplosiveChargeRotator = FRotator(E_SpawnAngle, GetActorRotation().Yaw, GetActorRotation().Roll);
		FVector ExplosiveChargeVelocity = E_Speed * ExplosiveChargeRotator.Vector();
		
		E_ExplosiveChargeStorage = GetWorld()->SpawnActor<AOPTristanaExplosiveCharge>(E_ExplosiveChargeClass, ProjectileSpawnPoint->GetComponentLocation(), ExplosiveChargeRotator);
		if (E_ExplosiveChargeStorage)
		{
			E_ExplosiveChargeStorage->GetOPProjectileMovementComponent()->HomingAccelerationMagnitude = 100000.f;
			E_ExplosiveChargeStorage->GetOPProjectileMovementComponent()->bIsHomingProjectile = true;
			E_ExplosiveChargeStorage->GetOPProjectileMovementComponent()->HomingTargetComponent = Target->GetRootComponent();
		}
	}
}


void AOPTristana::R() //대구경 탄환 (Buster Shot)효과: 트리스타나가 강력한 탄환을 발사하여 대상에게 피해를 입히고, 대상을 멀리 밀어냅니다. 활성화 : 이 스킬은 적 챔피언을 밀어내는 데 효과적이며, 특히 위험한 적을 멀리 밀어내어 위기 상황에서 탈출하거나 적을 분리시키는 데 사용할 수 있습니다.
{
	Super::R();

	if (!bR) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(R_SpawnTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		R_BusterShot();
	}), 0.3f, false);

	SetbR_False();
	GetWorldTimerManager().SetTimer(R_CooldownTimerHandle, this, &AOPTristana::SetbR_True, GetR_Cooldown(), false);

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage, 1.0f);
	}
	
	StopChampionMovement();
    GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPTristana::ResetChampionMovement, 0.9f, false);
        
    SetbR_False();
    GetWorldTimerManager().SetTimer(R_CooldownTimerHandle, this, &AOPTristana::SetbR_True, R_Cooldown, false);
}

void AOPTristana::R_BusterShot()
{
	UE_LOG(LogTemp, Log, TEXT("Ult_BusterShot"));

	if (R_BusterShotClass == nullptr) return;

	FRotator BusterShotRotator = FRotator(R_Angle, GetActorRotation().Yaw, GetActorRotation().Roll);
	FVector BusterShotVelocity = R_Speed * BusterShotRotator.Vector();

	if (R_BusterShotStorage = GetWorld()->SpawnActor<AOPTristanaBusterShot>(R_BusterShotClass, ProjectileSpawnPoint->GetComponentLocation(), BusterShotRotator))
	{
		R_BusterShotStorage->GetOPProjectileMovementComponent()->Velocity = BusterShotVelocity;
		R_BusterShotStorage->SetOwner(this);
		GetCharacterMovement()->AddImpulse(-BusterShotRotator.Vector() * R_Rebound, true);
		// LaunchCharacter(-BusterShotRotator.Vector() * R_Rebound, true, true);
	}
}
