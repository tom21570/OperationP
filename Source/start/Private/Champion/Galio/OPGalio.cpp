// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Galio/OPGalio.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/Galio/OPGalioWindblast.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"

AOPGalio::AOPGalio()
{
	Q_Windblast_SpawnPoint_1 = CreateDefaultSubobject<USceneComponent>("Q_Windblast_SpawnPoint_1");
	Q_Windblast_SpawnPoint_1->SetupAttachment(GetRootComponent());
	Q_Windblast_SpawnPoint_2 = CreateDefaultSubobject<USceneComponent>("Q_Windblast_SpawnPoint_2");
	Q_Windblast_SpawnPoint_2->SetupAttachment(GetRootComponent());
}

void AOPGalio::BeginPlay()
{
	Super::BeginPlay();

	FVector Velocity = FVector::ZeroVector;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, Velocity, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 5000.f, GetActorRightVector().Length() * 1000.f, 0.5f);
	GetCharacterMovement()->AddImpulse(Velocity, true);
}

void AOPGalio::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	W_Timeline.TickTimeline(DeltaSeconds);
	if(bW_Charging)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), W_FinalDistance, 10, FColor::Red, false, 0.f);
	}

	if (bE_IsDashing)
	{
		E_Trace();
	}

	if (bR_IsInAir)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			R_OnLanding();
		}
	}

	// Current Gravity Field가 유효한지 확인
	if (CurrentGravityField && IsValid(CurrentGravityField))
	{
		// 중력 방향 계산: 캐릭터와 중력 필드의 위치 차이를 기반으로 계산
		FVector GravityDirection = CurrentGravityField->GetActorLocation() - GetActorLocation();
		GravityDirection.Normalize(); // 방향 벡터를 정규화

		// 캐릭터의 Character Movement Component에서 Set Gravity Direction 함수 호출
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			UFunction* SetGravityDirectionFunction = CharacterMovementComponent->FindFunction(TEXT("SetGravityDirection"));
			if (SetGravityDirectionFunction)
			{
				struct FSetGravityDirectionParams
				{
					FVector GravityDirection;
				};

				FSetGravityDirectionParams SetGravityParams;
				SetGravityParams.GravityDirection = GravityDirection;

				// 중력 방향을 캐릭터에 적용
				CharacterMovementComponent->ProcessEvent(SetGravityDirectionFunction, &SetGravityParams);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentGravityField is not set or invalid."));
	}
}



void AOPGalio::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		BasicAttack_Trace();
	}), 0.35f, false);

	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		if (bPassive)
		{
			bPassive = false;
			
			ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection("Passive", BasicAttack_AnimMontage);
		}

		else
		{
			switch (BasicAttack_ComboCount)
			{
			case 0:
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("1"), BasicAttack_AnimMontage);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPGalio::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount++;
				break;

			case 1:
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("2"), BasicAttack_AnimMontage);
				GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_TimerHandle);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPGalio::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount = 0;
				break;
				
			case 2:
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("3"), BasicAttack_AnimMontage);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPGalio::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount++;
				break;

			case 3:
				ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage, 1.f);
				ChampionAnimInstance->Montage_JumpToSection(FName("4"), BasicAttack_AnimMontage);
				GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_TimerHandle);
				GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPGalio::BasicAttack_ResetComboCount, 5.f, false);
				BasicAttack_ComboCount = 0;
				break;
				
			default:
				;
			}
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPGalio::ResetChampionMovement, 1.7f, false);
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPGalio::SetbBasicAttack_True, BasicAttack_Cooldown, false);
}

void AOPGalio::BasicAttack_Trace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (bPassive)
			{
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
			}
			else
			{
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
			}
		}
	}
}

void AOPGalio::BasicAttacK_Trace_Passive()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Width,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
			}
		}
	}
}

void AOPGalio::Q()
{
	Super::Q();

	if (!bQ) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);
	Q_FinalLocation = MouseCursorHit.Location;

	GetWorldTimerManager().SetTimer(Q_Cast_TimerHandle, this, &AOPGalio::Q_SpawnWindblast, 0.25f, false);

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Q_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPGalio::ResetChampionMovement, 0.7f, false);

	SetbQ_False();
	GetWorldTimerManager().SetTimer(Q_Cooldown_TimerHandle, this, &AOPGalio::SetbQ_True, Q_Cooldown, false);
}

void AOPGalio::Q_SpawnWindblast()
{
	if (Q_Windblast_Class == nullptr) return;
	Q_Windblast_Storage_1 = GetWorld()->SpawnActor<AOPGalioWindblast>(Q_Windblast_Class, Q_Windblast_SpawnPoint_1->GetComponentLocation(), Q_Windblast_SpawnPoint_1->GetComponentRotation());
	Q_Windblast_Storage_1->SetOwner(this);
	Q_Windblast_Storage_2 = GetWorld()->SpawnActor<AOPGalioWindblast>(Q_Windblast_Class, Q_Windblast_SpawnPoint_2->GetComponentLocation(), Q_Windblast_SpawnPoint_2->GetComponentRotation());
	Q_Windblast_Storage_2->SetOwner(this);

	FVector WindblastVector1 = FVector::ZeroVector;
	FVector WindblastVector2 = FVector::ZeroVector;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, WindblastVector1, Q_Windblast_SpawnPoint_1->GetComponentLocation(), Q_FinalLocation, GetWorld()->GetGravityZ(), 0.7f);
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, WindblastVector2, Q_Windblast_SpawnPoint_2->GetComponentLocation(), Q_FinalLocation, GetWorld()->GetGravityZ(), 0.7f);
	Q_Windblast_Storage_1->GetOPProjectileMovementComponent()->Velocity = WindblastVector1;
	Q_Windblast_Storage_2->GetOPProjectileMovementComponent()->Velocity = WindblastVector2;
	
}

void AOPGalio::W()
{
	Super::W();

	if(!bW) return;

	if(!bW_Charging)
	{
		bW_Charging = true;
		W_Charge();

		if(ChampionAnimInstance && W_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(W_AnimMontage);
			ChampionAnimInstance->Montage_JumpToSection(FName(TEXT("Charge")), W_AnimMontage);
		}

		GetWorldTimerManager().SetTimer(W_CastTimer, this, &AOPGalio::W_Recast, 2.5f, false);
	}

	else if(bW_Charging)
	{
		W_Recast();
	}
}

void AOPGalio::W_Charge()
{
	if(W_ChargeCurve)
	{
		W_StartDistance = 175.f;
		W_EndDistance = 350.f;
		FOnTimelineFloat W_TimelineFloat;
		W_TimelineFloat.BindUFunction(this, FName(TEXT("W_TimelineProgress")));

		W_Timeline.AddInterpFloat(W_ChargeCurve, W_TimelineFloat);

		W_Timeline.PlayFromStart();
	}
}

void AOPGalio::W_Recast()
{
	bW_Charging = false;
	W_Timeline.Stop();
	GetWorldTimerManager().ClearTimer(W_CastTimer);
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(),
		GetActorLocation(), W_FinalDistance,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
		ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
			}
		}
	}
	
	if(ChampionAnimInstance && W_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(W_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection(FName(TEXT("Recast")), W_AnimMontage);
	}
	
	SetbW_False();
	GetWorldTimerManager().SetTimer(W_Cooldown_TimerHandle, this, &AOPGalio::SetbW_True, W_Cooldown, false);
}

void AOPGalio::W_TimelineProgress(float Value)
{
	if(bW_Charging)
	{
		float CurrentDistance = FMath::Lerp(W_StartDistance, W_EndDistance, Value);
		W_FinalDistance = CurrentDistance;
	}
}

void AOPGalio::E()
{
	Super::E();

	if (!bE) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	GetWorldTimerManager().SetTimer(E_DashStart_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		bE_IsDashing = true;
		LaunchCharacter(E_Speed * GetActorForwardVector(), true, true);
	}), 0.5f, false);
	
	LaunchCharacter(-E_Speed_Backward * GetActorForwardVector(), true, true);

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection("Dash", E_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPGalio::ResetChampionMovement, 1.5f, false);

	SetbE_False();
	GetWorldTimerManager().SetTimer(E_Cooldown_TimerHandle, this, &AOPGalio::SetbE_True, E_Cooldown, false);
}

void AOPGalio::E_Trace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation(), E_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitResult.GetActor()))
	{
		bE_IsDashing = false;
		HitDiavolo->GetCharacterMovement()->AddImpulse(HitDiavolo->GetActorUpVector() * E_Strength, true);
		HitDiavolo->SetbIsDamagedTrue();
		HitDiavolo->PlayDiavoloRandomDeadMontage();

		if(ChampionAnimInstance && E_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(E_AnimMontage);
			ChampionAnimInstance->Montage_JumpToSection(FName(TEXT("Hit")), E_AnimMontage);
		}
	}
}

void AOPGalio::R()
{
	Super::R();

	if (!bR) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	R_FinalLocation = MouseCursorHit.Location;

	GetWorldTimerManager().SetTimer(R_Departure_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		bR_IsInAir = true;
		LaunchCharacter(R_Speed_Z_Departure * GetActorUpVector(), true, true);
	}), 1.f, false);

	GetWorldTimerManager().SetTimer(R_StartLanding_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		LaunchCharacter(-R_Speed_Z_Landing * GetActorUpVector() + R_Speed_XY_Landing * GetActorForwardVector(), true, true);
	}), 2.f, false);

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection("Start", R_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPGalio::ResetChampionMovement, 3.5f, false);

	SetbR_False();
	GetWorldTimerManager().SetTimer(R_Cooldown_TimerHandle, this, &AOPGalio::SetbR_True, R_Cooldown, false);
}

void AOPGalio::R_Trace()
{
}

void AOPGalio::R_OnLanding()
{
	bR_IsInAir = false;
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), R_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			HitDiavolo->SetbIsDamagedTrue();
			HitDiavolo->PlayDiavoloRandomDeadMontage();
		}
	}

	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage);
		ChampionAnimInstance->Montage_JumpToSection("Land", R_AnimMontage);
	}
}
