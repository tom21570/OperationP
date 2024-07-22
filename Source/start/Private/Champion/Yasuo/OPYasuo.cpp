// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Yasuo/OPYasuo.h"
#include "Animation/OPAnimInstance.h"
#include "Champion/Yasuo/OPYasuoWhirlWind.h"
#include "Champion/Yasuo/OPYasuoWindWall.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"
#include "Sound/SoundCue.h"

AOPYasuo::AOPYasuo()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // 투사체 움직임 포인터에 동적 할당
}

void AOPYasuo::BeginPlay()
{
	Super::BeginPlay();
}

void AOPYasuo::Passive()
{
	Super::Passive();
}

void AOPYasuo::MeleeAttack()
{
	Super::MeleeAttack();

	if(!GetbMeleeAttack()) return; // 평타 쿨타임 시 return
	if(!GetOPPlayerController()) return; // 플레이어 컨트롤러가 nullptr 시 return

	// ECC_Visibility 채널에 대한 반응이 overlap 또는 block인 액터에 hit 했을 시 GetHitResultUnderCursor는 그 액터에 대한 HitResult를 MouseCursorHit에 저장.
	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if(MouseCursorHit.bBlockingHit) // 만약 반응이 block이라면 그 Hit 방향으로 캐릭터를 돌림
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	// 평타 쿨타임 설정을 위한 두 줄
	SetbMeleeAttack_False();
	GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPYasuo::SetbMeleeAttack_True, GetMeleeAttackCooltime(), false);

	// 평타 시전시간 지나면 Trace하고 디아볼로가 Trace되면 피격 사운드 재생
	GetWorldTimerManager().SetTimer(MeleeAttackCastTimer, FTimerDelegate::CreateLambda([&]
	{
		if (MeleeAttackTrace())
		{
			if (MeleeAttack_Hit_SFX)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), MeleeAttack_Hit_SFX);
			}
		};
	}), 0.25f, false);

	if (!ChampionAnimInstance) return; // 애니메이션 인스턴스가 없을 시 return
	if (!MeleeAttackAnimMontage) return; // 평타 애니메이션 몽타주가 없을 시 return
	
	switch (MeleeAttackComboCount) // 4번의 연결된 평타동작
	{
	case 0:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("1"), MeleeAttackAnimMontage);
		GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPYasuo::ResetMeleeAttackComboCount, 5.f, false);
		MeleeAttackComboCount++;
		break;

	case 1:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("2"), MeleeAttackAnimMontage);
		GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimer);
		GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPYasuo::ResetMeleeAttackComboCount, 5.f, false);
		MeleeAttackComboCount++;
		break;

	case 2:
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("3"), MeleeAttackAnimMontage);
		GetWorldTimerManager().ClearTimer(MeleeAttackComboCountTimer);
		GetWorldTimerManager().SetTimer(MeleeAttackComboCountTimer, this, &AOPYasuo::ResetMeleeAttackComboCount, 5.f, false);
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
}

bool AOPYasuo::MeleeAttackTrace()
{
	TArray<FHitResult> HitResults; // Hit의 결과를 저장할 배열
	TArray<AActor*> ActorsToIgnore; // 트레이스하지 않을 액터들
	ActorsToIgnore.Add(this); // 트레이스하지 않을 액터에 야스오 본인을 포함

	// 원 모양으로 어디부터 어디까지, 어떤 채널에서 트레이스할지 정하고 트레이스하는 함수
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 450.f, 80.f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	for (auto& HitActor : HitResults) // 트레이스된 여러 액터들에 적용하기 위한 반복문
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor())) // 트레이스된 액터를 디아볼로로 캐스트
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * MeleeAttack_Impulse, true); // 디아볼로에 충격 가하기
			if (!Diavolo->GetbCanBeTestedMultipleTimes()) // 만약 bCanBeTestedMultipleTimes가 false라면 더이상 트레이스되지 않도록 디아볼로를 설정
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}

			return true;
		}
	}

	return false;
}

void AOPYasuo::Skill_1()
{
	Super::Skill_1();
	
	if(!GetbSkill_1()) return;
	if(!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if(!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	if(Skill_1_Stack == 2) // 강철폭풍 스택이 2일때 강철폭풍 사용 시 스택을 0으로 초기화하고 회오리 날리기
	{
		UE_LOG(LogTemp, Log, TEXT("Skill_1_WhirlWind"));
		GetWorldTimerManager().SetTimer(WhirlWindSpawnTimer, this, &AOPYasuo::Skill_1_WhirlWind, 0.25f, false);
		GetWorldTimerManager().ClearTimer(Skill_1_StackTimer);
		Skill_1_Stack = 0;
		if (ChampionAnimInstance && Skill_1_AnimMontage)
		{
			GetChampionAnimInstance()->Montage_Play(GetSkill_1_AnimMontage(), 1.0f);
			GetChampionAnimInstance()->Montage_JumpToSection(FName("Wind"), GetSkill_1_AnimMontage());
		}
	}

	else
	{
		GetWorldTimerManager().SetTimer(Skill_1_CastTimer, FTimerDelegate::CreateLambda([&]
		{
			if (Skill_1_Trace())
			{
				if (Skill_1_Hit_SFX)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), Skill_1_Hit_SFX);
				}

				if (Skill_1_Stack == 2 && Skill_1_Charged_SFX)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), Skill_1_Charged_SFX);
				}
			}
		}), 0.2f, false);

		if (ChampionAnimInstance && Skill_1_AnimMontage)
		{
			int32 Section = FMath::RandRange(0, 1);
			switch (Section)
			{
			case 0:
				GetChampionAnimInstance()->Montage_Play(GetSkill_1_AnimMontage(), 1.0f);
				GetChampionAnimInstance()->Montage_JumpToSection(FName("1"), GetSkill_1_AnimMontage());
				break;

			case 1:
				GetChampionAnimInstance()->Montage_Play(GetSkill_1_AnimMontage(), 1.0f);
				GetChampionAnimInstance()->Montage_JumpToSection(FName("2"), GetSkill_1_AnimMontage());
				break;

			default:
				;
			}
		}
	}
	
	SetbSkill_1_False();
	GetWorldTimerManager().SetTimer(Skill_1_CooltimeTimer, this, &AOPYasuo::SetbSkill_1_True, GetSkill_1_Cooltime(), false);
}

bool AOPYasuo::Skill_1_Trace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 450.f, 40.f,
	UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * Skill_1_Impulse, true);
			Diavolo->TurnCharacterToLocation(GetActorLocation());
			
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
	
	if(HitResults.Num() > 0 && Skill_1_Stack < 2) // 1명이라도 맞추고 스택이 2 미만이라면
	{
		Skill_1_Stack++;
		GetWorldTimerManager().ClearTimer(Skill_1_StackTimer);
		GetWorldTimerManager().SetTimer(Skill_1_StackTimer, FTimerDelegate::CreateLambda([&]
		{
			Skill_1_Stack = 0;
			UE_LOG(LogTemp, Warning, TEXT("Stack Reset"));
		}), 6.f, false);

		return true;
	}

	return false;
}

void AOPYasuo::Skill_1_WhirlWind() // 회오리 발사
{
	if (WhirlWindClass == nullptr) return;
	// 현재 위치와 방향을 가져옵니다.
	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	// ForwardVector에 적절한 거리를 곱하여 정면 조금 앞의 위치를 계산합니다.
	// 여기서 100.0f는 스폰할 거리로, 원하는 대로 조정할 수 있습니다.
	FVector SpawnLocation = CurrentLocation + ForwardVector * 100.0f;

	WhirlWind = GetWorld()->SpawnActor<AOPYasuoWhirlWind>(WhirlWindClass, SpawnLocation, GetActorRotation());
	WhirlWind->SetOwner(this);
}

void AOPYasuo::Skill_2()
{
	Super::Skill_2();
	
	if (bSkill_2) return;
	if (!OPPlayerController) return;
	
	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	Skill_2_WindWall();

	if (ChampionAnimInstance && Skill_2_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Skill_2_AnimMontage, 1.0f);
	}

	SetbSkill_2_False();
	GetWorldTimerManager().SetTimer(Skill_2_CooltimeTimer, this, &AOPYasuo::SetbSkill_2_True, GetSkill_2_Cooltime(), false);
}

void AOPYasuo::Skill_2_WindWall()
{
	if (WindWallClass == nullptr) return;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f; // 캐릭터보다 살짝 앞에 스폰
	FRotator SpawnRotation = GetActorRotation();
	
	if (WindWall = GetWorld()->SpawnActor<AOPYasuoWindWall>(WindWallClass, SpawnLocation, SpawnRotation))
	{
		WindWall->SetOwner(this);
		FVector LaunchDirection = GetActorForwardVector();
	}
}

void AOPYasuo::Skill_3()
{
	Super::Skill_3();
	
	if (!bSkill_3) return;
	UE_LOG(LogTemp, Warning, TEXT("Skill 3 tasuo"));
	if (OPPlayerController == nullptr) return;
	
	if (ChampionAnimInstance && Skill_3_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Skill_3_AnimMontage, 1.0f);
	}

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	
	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;
	
	int32 Distance = FVector::Dist(GetActorLocation(), TestDiavolo->GetActorLocation());

	if (Distance <= Skill_3_Distance)
	{
		FVector Skill3Vector = MouseCursorHit.Location - GetActorLocation();
		Skill3Vector.Normalize();
		Skill3Vector *= Skill_3_Velocity;
		Skill3Vector.Z = 0.f;
		TurnCharacterToLocation(TestDiavolo->GetActorLocation());
		ProjectileMovementComponent->Velocity = Skill3Vector;
		SetActorEnableCollision(false);
		
		GetWorldTimerManager().SetTimer(Skill_3_EndTimer, FTimerDelegate::CreateLambda([&]
		{
			ProjectileMovementComponent->Velocity = FVector(0, 0, 0);
			SetActorEnableCollision(true);
			TestDiavolo->SetbIsDamagedTrue();
			TestDiavolo->PlayDiavoloRandomDeadMontage();
			if (TestDiavolo->GetbCanBeTestedMultipleTimes() == false)
			{
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
			TestDiavolo = nullptr;
		}), 475.f/750.f, false);
	}
	
	SetbSkill_3_False();
	GetWorldTimerManager().SetTimer(Skill_3_CooltimeTimer, this, &AOPYasuo::SetbSkill_3_True, Skill_3_Cooltime, false);
}

void AOPYasuo::Skill_4()
{
	Super::Skill_4();

	if (!GetbSkill_4()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;

	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;

	int32 Distance = FVector::Dist(GetActorLocation(), TestDiavolo->GetActorLocation());
	FTimerHandle Skill_4_EndTimer;

	if (Distance <= Skill_4_Distance)
	{
		FVector Skill4Vector = MouseCursorHit.Location - GetActorLocation();
		Skill4Vector.Normalize();
		Skill4Vector *= Skill_4_Velocity;
		Skill4Vector.Z = 0.f;
		TurnCharacterToLocation(TestDiavolo->GetActorLocation());
		SetActorEnableCollision(true); // Ensure collision is enabled
		// Set the velocity for the projectile movement
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->Velocity = Skill4Vector;
			ProjectileMovementComponent->bSweepCollision = true;
			ProjectileMovementComponent->Activate();
			
			// Bind the collision event
			GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AOPYasuo::OnProjectileHit);
		}
	}

	GetWorldTimerManager().SetTimer(Skill_4_EndTimer, FTimerDelegate::CreateLambda([&]
	{
			if (ProjectileMovementComponent)
			{
				ProjectileMovementComponent->Velocity = FVector::ZeroVector;
				ProjectileMovementComponent->Deactivate();
			}
			
			TestDiavolo->SetbIsDamagedTrue();
			TestDiavolo->PlayDiavoloRandomDeadMontage();
			if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
			{
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}), 475.f / 750.f, false);

	check(GetChampionAnimInstance());
	check(GetSkill_3_AnimMontage());

	GetChampionAnimInstance()->Montage_Play(GetSkill_3_AnimMontage(), 1.0f);

	SetbSkill_4_False();
	GetWorldTimerManager().SetTimer(Skill_4_CooltimeTimer, this, &AOPYasuo::SetbSkill_4_True, GetSkill_4_Cooltime(), false);

}

// Collision event handler
void AOPYasuo::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnProjectileHit Active 1"));
	UE_LOG(LogTemp, Warning, TEXT("HitComponent: %s / OtherActor: %s / OtherComp: %s / Hit: %s"),
		*HitComponent->GetName(),
		*OtherActor->GetName(),
		*OtherComp->GetName(),
		*Hit.ToString());
	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnProjectileHit Active 2"));
		
		if (TestDiavolo)
		{            // Calculate the direction of the impulse
			FVector ImpactDirection = (TestDiavolo->GetActorLocation() - Hit.ImpactPoint).GetSafeNormal();

			// Add an upward component to the impact direction
			ImpactDirection.Z += Skill_4_Angle;
			ImpactDirection = ImpactDirection.GetSafeNormal();

			// Log the impact direction for debugging
			UE_LOG(LogTemp, Warning, TEXT("Impact Direction: %s"), *ImpactDirection.ToString());

			// Apply an impulse to the Diavolo character based on the impact direction and AirborneRate
			TestDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * Skill_4_Impulse, true);

			if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
			{
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}

			TestDiavolo = nullptr;
		}
		
	}
	
}

void AOPYasuo::Ult()
{
	Super::Ult();
	
	if (!bUlt) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);
	
	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;
	if (!TestDiavolo->GetChampionAnimInstance()->GetbIsInAir()) return;

	int32 Distance = FVector::Dist(GetActorLocation(), TestDiavolo->GetActorLocation());

	if (Distance <= 1400.f)
	{
		FVector DiavoloLocation = TestDiavolo->GetActorLocation();
		DiavoloLocation.Z -= 200.f;
		DiavoloLocation -= GetActorForwardVector() * 200.f;
		SetActorLocation(DiavoloLocation);
		TestDiavolo->GetCharacterMovement()->DisableMovement();
		// GetCharacterMovement()->AddImpulse(FVector(0.f, 0.f, 1000.f), true);
		GetCharacterMovement()->GravityScale = 0.f;
		
		GetWorldTimerManager().SetTimer(Ult_End_Timer, FTimerDelegate::CreateLambda([&]
		{
			GetCharacterMovement()->GravityScale = 1.f;
			TestDiavolo->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			TestDiavolo->GetCharacterMovement()->AddImpulse(FVector(0.f, 0.f, -Ult_Impulse), true);
			TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			GetCharacterMovement()->AddImpulse(FVector(0.f, 0.f, -Ult_Impulse), true);
			TestDiavolo->PlayDiavoloRandomDeadMontage();
			TestDiavolo = nullptr;
		}), 1.f, false);
	}


	if (ChampionAnimInstance && Ult_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Ult_AnimMontage, 1.0f);
	}
	
	SetbUlt_False();
	GetWorldTimerManager().SetTimer(Ult_CooltimeTimer, this, &AOPYasuo::SetbUlt_True, GetUlt_Cooltime(), false);
}


