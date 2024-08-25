// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Volibear/OPVolibear.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
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

	E_LightningShield_NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("E_LightningShield_NiagaraComponent"));
	E_LightningShield_NiagaraComponent->SetupAttachment(GetRootComponent());
}

void AOPVolibear::BeginPlay()
{
	Super::BeginPlay();
	E_LightningShield_NiagaraComponent->SetVisibility(false);
}

void AOPVolibear::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bR_IsJumping)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			R_OnLanding();
			UE_LOG(LogTemp, Warning, TEXT("Falling End"));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), R_LightningEffect, GetActorLocation());
			bR_IsJumping = false;
		}
	}
}

void AOPVolibear::Passive()
{
	Super::Passive();
}

void AOPVolibear::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		if (bQ_ThunderingSmash)
		{
			GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
			{
				BasicAttack_Trace_Q();
			}), 0.5f, false);

			bQ_ThunderingSmash = false;
			WalkSpeed = DefaultWalkSpeed;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			
			ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection("Thundering Smash", BasicAttack_AnimMontage);
			StopChampionMovement();
			GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPVolibear::ResetChampionMovement, 2.5f, false);
		}

		else
		{
			GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
			{
				BasicAttack_Trace();
			}), 0.25f, false);
			
			switch (BasicAttack_ComboCount)
			{
			case 0:
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("1"), BasicAttack_AnimMontage);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPVolibear::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount++;
				break;

			case 1:
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("2"), BasicAttack_AnimMontage);
				GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_TimerHandle);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPVolibear::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount = 0;
				break;
				
			default:
				;
			}

			StopChampionMovement();
			GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPVolibear::ResetChampionMovement, 1.05f, false);
		}
	}
	
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPVolibear::SetbBasicAttack_True, BasicAttack_Cooldown, false);
}

void AOPVolibear::BasicAttack_Trace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitResult.GetActor()))
	{
		Diavolo->SetbIsDamagedTrue();
		Diavolo->PlayDiavoloRandomDeadMontage();
		Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * BasicAttack_Strength, true);
		if (!Diavolo->GetbCanBeTestedMultipleTimes())
		{
			Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
	}
}

void AOPVolibear::BasicAttack_Trace_Q()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitResult.GetActor()))
	{
		// Diavolo->GetChampionAnimInstance()->SetbIsInAir_True();
		// Diavolo->GetChampionAnimInstance()->SetbIsInAir_False();
		// Diavolo->SetbIsDamagedTrue();
		// Diavolo->PlayDiavoloRandomDeadMontage();
		Diavolo->GetCharacterMovement()->AddImpulse(GetActorUpVector() * Q_Strength_Z + GetActorForwardVector() * Q_Strength_XY, true);
		if (!Diavolo->GetbCanBeTestedMultipleTimes())
		{
			Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
	}
}

void AOPVolibear::Q() //번개 강타 Q 볼리베어가 적을 향해 이동할 때 이동 속도가 증가하며 처음으로 기본 공격하는 대상을 기절시키고 피해를 입힙니다.
{
	Super::Q(); 

	if (!bQ) return;
	if (OPPlayerController == nullptr) return;

	bQ_ThunderingSmash = true;
	WalkSpeed = Q_WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	GetWorldTimerManager().SetTimer(Q_End_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		bQ_ThunderingSmash = false;
		WalkSpeed = DefaultWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}), Q_MaintainTime, false);
	
	SetbQ_False();
	GetWorldTimerManager().SetTimer(W_Cooldown_TimerHandle, this, &AOPVolibear::SetbQ_True, Q_Cooldown, false);
}

void AOPVolibear::W() //광란의 상처 W 볼리베어가 적에게 피해를 입혀 적중 시 효과를 적용하고 표식을 남깁니다.표식을 남긴 대상에게 다시 이 스킬을 사용하면 추가 피해를 입히고 체력을 회복합니다.
{
	Super::W();

	if (!bW) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	ChampionAnimInstance->Montage_Play(W_AnimMontage, 1.f);
	
	if (W_TraceForMaul())
	{
		ChampionAnimInstance->Montage_JumpToSection(FName("Reinforced"), W_AnimMontage);
	}
	else
	{
		ChampionAnimInstance->Montage_JumpToSection(FName("Default"), W_AnimMontage);
	}

	GetWorldTimerManager().SetTimer(W_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		W_Trace();
	}), 0.25f, false);
	
	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPVolibear::ResetChampionMovement, 1.f, false);

	SetbW_False();
	GetWorldTimerManager().SetTimer(W_Cooldown_TimerHandle, this, &AOPVolibear::SetbW_True, W_Cooldown, false);
}

bool AOPVolibear::W_TraceForMaul()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * W_Range, W_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitResult.GetActor()))
	{
		if (Diavolo->GetbFrenziedMaulOn())
		{
			return true;
		}
	}
	return false;
}

void AOPVolibear::W_Trace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * W_Range, W_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitResult.GetActor()))
	{
		if (!Diavolo->GetbFrenziedMaulOn())
		{
			Diavolo->SetbFrenziedMaulOn_True();
		}

		else if (Diavolo->GetbFrenziedMaulOn())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), W_BloodNiagara, Diavolo->GetActorLocation());
		}
		
		Diavolo->SetbIsDamagedTrue();
		Diavolo->PlayDiavoloRandomDeadMontage();
		Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * W_Strength, true);
		if (!Diavolo->GetbCanBeTestedMultipleTimes())
		{
			Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
	}
}

void AOPVolibear::E() //천공 분열E 볼리베어가 지정한 위치에 번개를 소환해 적에게 피해를 입히고 둔화시킵니다.볼리베어가 폭발 반경 안에 있으면 보호막을 얻습니다.
{
	Super::E();

	if (!bE) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	E_FinalLocation = MouseCursorHit.Location;

	GetWorldTimerManager().SetTimer(E_LightningboltSpawn_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		E_Lightningbolt();
	}), 2.f, false);

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage, 1.f);
	}
	
	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPVolibear::ResetChampionMovement, 2.6f, false);

	SetbE_False();
	GetWorldTimerManager().SetTimer(E_Cooldown_TimerHandle, this, &AOPVolibear::SetbE_True, E_Cooldown, false);
}

void AOPVolibear::E_Lightningbolt() //
{
	if (E_LightningboltClass)
	{
		E_Lightningbolt_Trace();
		E_LightningboltStorage = GetWorld()->SpawnActor<AOPVolibearLightningbolt>(E_LightningboltClass, E_FinalLocation, GetActorRotation());
		E_LightningboltStorage->SetOwner(this);
	}
}

void AOPVolibear::E_Lightningbolt_Trace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), E_FinalLocation, E_FinalLocation, E_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, false);
	
	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				FRotator ExplosionRotation = (HitDiavolo->GetActorLocation() - E_FinalLocation).Rotation();
				FRotator FinalRotation = FRotator(E_StrengthAngle, ExplosionRotation.Yaw, ExplosionRotation.Roll);
				HitDiavolo->GetCharacterMovement()->AddImpulse(FinalRotation.Vector() * E_Strength, true);
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
			}
		}

		if (AOPVolibear* HitVolibear = Cast<AOPVolibear>(HitActor.GetActor()))
		{
			HitVolibear->E_LightningShield_NiagaraComponent->SetVisibility(true);
			UE_LOG(LogTemp, Warning, TEXT("Hello"));
		}
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

	R_FinalLocation = MouseCursorHit.Location;
	FVector UltVector = R_FinalLocation - GetActorLocation();
	
	FVector UltVector_XY = UltVector.GetSafeNormal();
	UltVector_XY.Z = 0.f;

	LaunchCharacter(UltVector_XY * R_Velocity_XY + GetActorUpVector() * R_Velocity_Z, true, true); // 속도

	bR_IsJumping = true;

	GetMesh()->SetWorldScale3D(FVector(R_SizeIncreaseIndex, R_SizeIncreaseIndex, R_SizeIncreaseIndex));

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage, 1.f);
	}

	GetWorldTimerManager().SetTimer(R_End_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		GetMesh()->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	}), 12.f, false);

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPVolibear::ResetChampionMovement, 2.f, false);

	SetbR_False();
	GetWorldTimerManager().SetTimer(R_Cooldown_TimerHandle, this, &AOPVolibear::SetbR_True, R_Cooldown, false);

	// ProjectileMovementComponent->Velocity = GetActorForwardVector() * 100000.f;
	
	// Launch 방식
	// if (bUlt_ActAsProjectile)
	// {
	// 	
	// }

	// // Game 방식
	// if (bUlt_ActAsGame)
	// {
	// 	if (ProjectileMovementComponent == nullptr) return;
	// 	
	// 	const FVector FinalVelocity = GetActorForwardVector() * R_Velocity_XY + GetActorUpVector() * R_Velocity_Z;;
	//
	// 	ProjectileMovementComponent->Velocity = FinalVelocity;
	// 	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	// 	GetWorldTimerManager().SetTimer(Ult_StopTimerHandle, FTimerDelegate::CreateLambda([&]
	// 	{
	// 		ProjectileMovementComponent->Velocity = FVector::Zero();
	// 		ProjectileMovementComponent->ProjectileGravityScale = 1.f;
	// 	}), 0.1f, false);
	// }
	//
	// // 포물선 운동 방식
	// else if (bUlt_ActAsParabola)
	// {
	// 	if (ProjectileMovementComponent == nullptr) return;
	// 	
	// 	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, Ult_Velocity_Parabola, GetActorLocation(), Ult_FinalLocation, 1.f, 0.5);
	// 	GetCharacterMovement()->AddImpulse(Ult_Velocity_Parabola, true);
	// }
}

void AOPVolibear::R_OnLanding()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), R_LandingRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				FRotator ExplosionRotation = (HitDiavolo->GetActorLocation() - GetActorLocation()).Rotation();
				FRotator FinalRotation = FRotator(R_LandingStrengthAngle, ExplosionRotation.Yaw, ExplosionRotation.Roll);
				HitDiavolo->GetCharacterMovement()->AddImpulse(FinalRotation.Vector() * R_LandingStrength, true);
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
			}
		}
	}
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
