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

	Q_WhirlWindSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Whirl Wind Spawn Point"));
	Q_WhirlWindSpawnPoint->SetupAttachment(GetRootComponent());
}

void AOPYasuo::BeginPlay()
{
	Super::BeginPlay();
}

void AOPYasuo::Passive()
{
	Super::Passive();
}

void AOPYasuo::BasicAttack()
{
	Super::BasicAttack();

	if(!bBasicAttack) return; // 평타 쿨타임 시 return
	if(OPPlayerController == nullptr) return; // 플레이어 컨트롤러가 nullptr 시 return

	// ECC_Visibility 채널에 대한 반응이 overlap 또는 block인 액터에 hit 했을 시 GetHitResultUnderCursor는 그 액터에 대한 HitResult를 MouseCursorHit에 저장.
	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return; 
	TurnCharacterToCursor(MouseCursorHit); // 만약 반응이 block이라면 그 Hit 방향으로 캐릭터를 돌림

	// 평타를 치고 1.4초 동안은 움직일 수 없게 함
	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPYasuo::ResetChampionMovement, 1.4f, false);

	// 평타 쿨타임 설정
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttackCooltimeTimerHandle, this, &AOPYasuo::SetbBasicAttack_True, GetBasicAttackCooltime(), false);

	// 평타 시전시간 지나면 Trace하고 디아볼로가 Trace되면 피격 사운드 재생
	GetWorldTimerManager().SetTimer(BasicAttack_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		if (BasicAttackTrace())
		{
			if (BasicAttack_Hit_SFX)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), BasicAttack_Hit_SFX);
			}
		};
	}), 0.25f, false);

	if (!ChampionAnimInstance) return; // 애니메이션 인스턴스가 없을 시 return
	if (!BasicAttackAnimMontage) return; // 평타 애니메이션 몽타주가 없을 시 return

	if (ChampionAnimInstance && BasicAttackAnimMontage)
	{
		switch (BasicAttackComboCount) // 4번의 연결된 평타동작
		{
		case 0:
			ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection(FName("1"), BasicAttackAnimMontage);
			GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPYasuo::ResetBasicAttackComboCount, 5.f, false);
			BasicAttackComboCount++;
			break;

		case 1:
			ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection(FName("2"), BasicAttackAnimMontage);
			GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_TimerHandle);
			GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPYasuo::ResetBasicAttackComboCount, 5.f, false);
			BasicAttackComboCount++;
			break;

		case 2:
			ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection(FName("3"), BasicAttackAnimMontage);
			GetWorldTimerManager().ClearTimer(BasicAttack_ComboCount_TimerHandle);
			GetWorldTimerManager().SetTimer(BasicAttack_ComboCount_TimerHandle, this, &AOPYasuo::ResetBasicAttackComboCount, 5.f, false);
			BasicAttackComboCount++;
			break;

		case 3:
			ChampionAnimInstance->Montage_Play(BasicAttackAnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection(FName("4"), BasicAttackAnimMontage);
			BasicAttackComboCount = 0;
			break;

		default:
			;
		}
	}
}

bool AOPYasuo::BasicAttackTrace()
{
	TArray<FHitResult> HitResults; // Hit의 결과를 저장할 배열
	TArray<AActor*> ActorsToIgnore; // 트레이스하지 않을 액터들
	ActorsToIgnore.Add(this); // 트레이스하지 않을 액터에 야스오 본인을 포함

	// 원 모양으로 어디부터 어디까지, 어떤 채널에서 트레이스할지 정하고 트레이스하는 함수
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * BasicAttack_Range, BasicAttack_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);
	
	for (auto& HitActor : HitResults) // 트레이스된 여러 액터들에 적용하기 위한 반복문
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor())) // 트레이스된 액터를 디아볼로로 캐스트
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * BasicAttack_Impulse, true); // 디아볼로에 충격 가하기
			if (!Diavolo->GetbCanBeTestedMultipleTimes()) // 만약 bCanBeTestedMultipleTimes가 false라면 더이상 트레이스되지 않도록 디아볼로를 설정
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}

			return true;
		}
	}

	return false;
}

void AOPYasuo::Q()
{
	Super::Q();
	
	if(!bQ) return;
	if(!OPPlayerController) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if(!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	if(Q_Stack == 2) // 강철폭풍 스택이 2일때 강철폭풍 사용 시 스택을 0으로 초기화하고 회오리 날리기
	{
		UE_LOG(LogTemp, Log, TEXT("Skill_1_WhirlWind"));
		GetWorldTimerManager().SetTimer(Q_WhirlWindSpawn_TimerHandle, this, &AOPYasuo::Q_WhirlWind, 0.25f, false);
		GetWorldTimerManager().ClearTimer(Q_Stack_TimerHandle);
		Q_Stack = 0;
		if (ChampionAnimInstance && Q_AnimMontage)
		{
			ChampionAnimInstance->Montage_Play(Q_AnimMontage, 1.0f);
			ChampionAnimInstance->Montage_JumpToSection(FName("Wind"), Q_AnimMontage);
		}
	}

	else
	{
		GetWorldTimerManager().SetTimer(Q_Cast_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			if (Q_Trace())
			{
				if (Q_Hit_SFX)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), Q_Hit_SFX);
				}

				if (Q_Stack == 2 && Q_Charged_SFX)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), Q_Charged_SFX);
				}
			}
		}), 0.2f, false);

		if (ChampionAnimInstance && Q_AnimMontage)
		{
			if (bQ_CanSwift) // Q CanSwift 불리언 값이 true라면 초당 6번 Q를 시전하는 애니메이션을 재생하도록 함.
			{
				Q_PlayOrdinaryAnimMontage();
				GetWorldTimerManager().SetTimer(Q_SwiftStart_TimerHandle, FTimerDelegate::CreateLambda([&]
				{
					Q_PlayOrdinaryAnimMontage();
					UE_LOG(LogTemp, Warning, TEXT("Hello"));
				}), 0.165f, true);

				GetWorldTimerManager().SetTimer(Q_SwiftEnd_TimerHandle, FTimerDelegate::CreateLambda([&]
				{
					GetWorldTimerManager().ClearTimer(Q_SwiftStart_TimerHandle);
				}), 1.f, false);
			}

			else
			{
				int32 Section = FMath::RandRange(0, 1);
				switch (Section)
				{
				case 0:
					ChampionAnimInstance->Montage_Play(GetQ_AnimMontage(), 1.0f);
					ChampionAnimInstance->Montage_JumpToSection(FName("1"), GetQ_AnimMontage());
					break;

				case 1:
					ChampionAnimInstance->Montage_Play(GetQ_AnimMontage(), 1.0f);
					ChampionAnimInstance->Montage_JumpToSection(FName("2"), GetQ_AnimMontage());
					break;

				default:
					;
				}
			}
		}
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPYasuo::ResetChampionMovement, 0.7f, false);
	
	SetbQ_False();
	GetWorldTimerManager().SetTimer(Q_CooldownTimerHandle, this, &AOPYasuo::SetbQ_True, GetQ_Cooldown(), false);
}

void AOPYasuo::Q_PlayOrdinaryAnimMontage()
{
	if (ChampionAnimInstance == nullptr) return;
	if (Q_Swift_AnimMontage == nullptr) return;
	
	int32 Section = FMath::RandRange(0, 1);
	switch (Section)
	{
	case 0:
		ChampionAnimInstance->Montage_Play(Q_Swift_AnimMontage, 6.0f);
		ChampionAnimInstance->Montage_JumpToSection(FName("1"), Q_Swift_AnimMontage);
		break;

	case 1:
		ChampionAnimInstance->Montage_Play(Q_Swift_AnimMontage, 6.0f);
		ChampionAnimInstance->Montage_JumpToSection(FName("2"), Q_Swift_AnimMontage);
		break;

	default:
		;
	}
}

bool AOPYasuo::Q_Trace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * Q_Range, Q_Radius,
	UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			Diavolo->SetbIsDamagedTrue();
			Diavolo->PlayDiavoloRandomDeadMontage();
			Diavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * Q_Strength, true);
			Diavolo->TurnCharacterToLocation(GetActorLocation());
			
			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
	
	if(HitResults.Num() > 0 && Q_Stack < 2) // 1명이라도 맞추고 스택이 2 미만이라면
	{
		Q_Stack++;
		GetWorldTimerManager().ClearTimer(Q_Stack_TimerHandle);
		GetWorldTimerManager().SetTimer(Q_Stack_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			Q_Stack = 0;
			UE_LOG(LogTemp, Warning, TEXT("Stack Reset"));
		}), 6.f, false);

		return true;
	}

	return false;
}

void AOPYasuo::Q_WhirlWind() // 회오리 발사
{
	if (Q_WhirlWindClass == nullptr) return;

	Q_WhirlWindStorage = GetWorld()->SpawnActor<AOPYasuoWhirlWind>(Q_WhirlWindClass, Q_WhirlWindSpawnPoint->GetComponentLocation(), GetActorRotation());
	Q_WhirlWindStorage->SetOwner(this);
}

void AOPYasuo::W()
{
	Super::W();
	
	if (!bW) return;
	if (!OPPlayerController) return;
	
	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	W_WindWall();

	if (ChampionAnimInstance && W_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(W_AnimMontage, 1.0f);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPYasuo::ResetChampionMovement, 0.4f, false);

	SetbW_False();
	GetWorldTimerManager().SetTimer(W_CooldownTimerHandle, this, &AOPYasuo::SetbW_True, GetW_Cooldown(), false);
}

void AOPYasuo::W_WindWall()
{
	if (W_WindWallClass == nullptr) return;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f; // 캐릭터보다 살짝 앞에 스폰
	FRotator SpawnRotation = GetActorRotation();
	
	if (W_WindWallStorage = GetWorld()->SpawnActor<AOPYasuoWindWall>(W_WindWallClass, SpawnLocation, SpawnRotation))
	{
		W_WindWallStorage->SetOwner(this);
		FVector LaunchDirection = GetActorForwardVector();
	}
}

void AOPYasuo::E()
{
	Super::E();
	
	if (!bE) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	
	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;
	
	E_DistanceBetweenEnemy = FVector::Dist(GetActorLocation(), TestDiavolo->GetActorLocation());

	FVector Skill3Vector = MouseCursorHit.Location - GetActorLocation();
	Skill3Vector.Normalize();
	Skill3Vector *= E_Speed;
	Skill3Vector.Z = 0.f;
	TurnCharacterToLocation(TestDiavolo->GetActorLocation());
	ProjectileMovementComponent->Velocity = Skill3Vector;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AOPYasuo::E_OnDrawingSword);
		
	GetWorldTimerManager().SetTimer(E_End_TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		ProjectileMovementComponent->Velocity = FVector(0, 0, 0);
		SetActorEnableCollision(true);
		GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &AOPYasuo::E_OnDrawingSword);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		TestDiavolo->SetbIsDamagedTrue();
		TestDiavolo->PlayDiavoloRandomDeadMontage();
		if (TestDiavolo->GetbCanBeTestedMultipleTimes() == false)
		{
			TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
		TestDiavolo = nullptr;
	}), E_CastTime, false);

	if (ChampionAnimInstance && E_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(E_AnimMontage, 1.0f);
	}
	
	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPYasuo::ResetChampionMovement, 0.7f, false);
	
	SetbE_False();
	GetWorldTimerManager().SetTimer(E_CooldownTimerHandle, this, &AOPYasuo::SetbE_True, E_Cooldown, false);
}

void AOPYasuo::E_OnDrawingSword(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AOPDiavolo* SlicedChampion = Cast<AOPDiavolo>(OtherActor);

		if (SlicedChampion)
		{
			FVector Direction = (SlicedChampion->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			SlicedChampion->LaunchCharacter(Direction * E_DistanceBetweenEnemy * E_Speed, true, true);
			UE_LOG(LogTemp, Warning, TEXT("Hello"));
		}
	}
}

void AOPYasuo::R()
{
	Super::R();
	
	if (!bR) return;
	if (OPPlayerController == nullptr) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);
	
	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;
	if (!TestDiavolo->GetChampionAnimInstance()->GetbIsInAir()) return;

	int32 Distance = FVector::Dist(GetActorLocation(), TestDiavolo->GetActorLocation());

	if (Distance <= 100000000.f)
	{
		FVector DiavoloLocation = TestDiavolo->GetActorLocation();
		// DiavoloLocation.Z -= 200.f;
		// DiavoloLocation -= GetActorForwardVector() * 200.f;
		SetActorLocation(DiavoloLocation);
		TestDiavolo->GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->GravityScale = 0.f;
		
		GetWorldTimerManager().SetTimer(R_End_TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			GetCharacterMovement()->GravityScale = 1.f;
			TestDiavolo->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			TestDiavolo->GetCharacterMovement()->AddImpulse(FVector(0.f, 0.f, -R_Strength), true);
			TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			GetCharacterMovement()->AddImpulse(FVector(0.f, 0.f, -R_Strength), true);
			TestDiavolo->PlayDiavoloRandomDeadMontage();
			TestDiavolo = nullptr;
		}), 1.f, false);
	}


	if (ChampionAnimInstance && R_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(R_AnimMontage, 1.0f);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPYasuo::ResetChampionMovement, 1.35f, false);
	
	SetbR_False();
	GetWorldTimerManager().SetTimer(R_CooldownTimerHandle, this, &AOPYasuo::SetbR_True, GetR_Cooldown(), false);
}
