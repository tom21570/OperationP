// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Malphite/OPMalphite.h"
#include "Animation/OPAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Diavolo/OPDiavolo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"

//#include "Champion/Yasuo/OPYasuoWhirlWind.h"//\말파이트 돌던지는 스킬로 교체
AOPMalphite::AOPMalphite()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // 투사체 움직임 포인터에 동적 할당
}

void AOPMalphite::BeginPlay()
{
	Super::BeginPlay();
}

void AOPMalphite::Passive()
{
	Super::Passive();
}

void AOPMalphite::MeleeAttack() //평타
{
	Super::MeleeAttack();

	if (!GetbMeleeAttack()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	check(GetChampionAnimInstance());
	check(GetMeleeAttackAnimMontage());

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]
	{
			MeleeAttackTrace();
	}), 0.25f, false);

	if (bThunderClapOn == true)
	{
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("clap"), MeleeAttackAnimMontage);
		bThunderClapOn = false;
	}
	else
	{
		GetChampionAnimInstance()->Montage_Play(MeleeAttackAnimMontage, 1.f);
	}


	SetbMeleeAttack_False();
	SetbMeleeAttack_False();
	GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPMalphite::SetbMeleeAttack_True, GetMeleeAttackCooltime(), false);
}

bool AOPMalphite::MeleeAttackTrace()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200.f, 80.f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		TestDiavolo = Cast<AOPDiavolo>(HitActor.GetActor());

		if (TestDiavolo)
		{
			TestDiavolo->SetbIsDamagedTrue();
			PlayDiavoloRandomDeadMontage();
			TestDiavolo->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * MeleeAttack_Impulse, true);
			if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
			{
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}

			return true;
		}
	}

	return false;
}

void AOPMalphite::Skill_1() //지진의 파동 (Seismic Shard): 설명: 말파이트가 지정한 적에게 바위를 던져 마법 피해를 입히고 이동 속도를 훔칩니다.
{
}

void AOPMalphite::Skill_2() //천둥의 파편 (Thunderclap): 설명: 말파이트의 다음 기본 공격이 추가 물리 피해를 입히고, 주변의 적들에게 추가 피해를 줍니다. 불주먹
{
	Super::Skill_2();

	bThunderClapOn = true;

	if (ChampionAnimInstance && Skill_2_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Skill_2_AnimMontage, 1.f);
	}

}

void AOPMalphite::Skill_3() //지진 강타 (Ground Slam): 설명: 말파이트가 지면을 강타하여 주위의 적들에게 물리 피해를 입히고 공격 속도를 감소시킵니다. 연관 함수 Skill_3_GroundSlam, Skill_3_ApplySlowAttackEffect
{
	Super::Skill_3();

	if (!GetbSkill_3()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
		Skill_3_GroundSlam();

		SetbSkill_3_False();
		GetWorldTimerManager().SetTimer(Skill_3_CooltimeTimer, this, &AOPMalphite::SetbSkill_3_True, GetSkill_3_Cooltime(), false);
	}
}


void AOPMalphite::Skill_3_GroundSlam()
{
	if (ChampionAnimInstance && Skill_3_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Skill_3_AnimMontage, 1.f);
		ChampionAnimInstance->Montage_JumpToSection(FName("GroundSlam"), Skill_3_AnimMontage);
	}

	Skill_3_ApplySlowAttackEffect();
}


void AOPMalphite::Skill_3_ApplySlowAttackEffect()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	float EffectRadius = Skill_3_radious;
	float SlowAmount = Skill_3_slowAmount;
	float SlowDuration = Skill_3_slowDuration;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * EffectRadius, EffectRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				HitDiavolo->SetbIsDamagedTrue();
				PlayDiavoloRandomDeadMontage();
				HitDiavolo->ApplySlowAttackEffect(SlowAmount, SlowDuration); //디아볼로에 공격모션이 느려지는 함수 구현필요
			}
		}
	}
}

void AOPMalphite::Skill_4() 
{
}

void AOPMalphite::Ult() //멈출 수 없는 힘 (Unstoppable Force): 설명: 말파이트가 지정한 위치로 돌진하여 도착 시 주변의 적들에게 마법 피해를 입히고 넉백시킵니다. //말파이트가 돌진을 안하는데, 블루프린트의 캡슐컴포넌트가 작아서 그런건지..
{
	Super::Ult();

	if (!GetbUlt()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;

	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;

	int32 Distance = FVector::Dist(GetActorLocation(), TestDiavolo->GetActorLocation());
	FTimerHandle Ult_EndTimer;

	if (Distance <= Ult_Distance)
	{
		FVector UltVector = MouseCursorHit.Location - GetActorLocation();
		UltVector.Normalize();
		UltVector *= Ult_Velocity;
		UltVector.Z = 0.f;
		TurnCharacterToLocation(TestDiavolo->GetActorLocation());
		SetActorEnableCollision(true); // Ensure collision is enabled
		// Set the velocity for the projectile movement
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->Velocity = UltVector;
			ProjectileMovementComponent->bSweepCollision = true;
			ProjectileMovementComponent->Activate();

			// Bind the collision event
			GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AOPMalphite::OnProjectileHit);
		}
	}

	GetWorldTimerManager().SetTimer(Ult_EndTimer, FTimerDelegate::CreateLambda([&]
		{

			if (ProjectileMovementComponent)
			{
				ProjectileMovementComponent->Velocity = FVector::ZeroVector;
				ProjectileMovementComponent->Deactivate();
			}

			TestDiavolo->SetbIsDamagedTrue();
			PlayDiavoloRandomDeadMontage();
			if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
			{
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}), 475.f / 750.f, false);

	check(GetChampionAnimInstance());
	check(GetUlt_AnimMontage());

	GetChampionAnimInstance()->Montage_Play(GetUlt_AnimMontage(), 1.0f);

	SetbUlt_False();
	GetWorldTimerManager().SetTimer(Ult_CooltimeTimer, this, &AOPMalphite::SetbUlt_True, GetUlt_Cooltime(), false);
}

// Collision event handler
void AOPMalphite::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
			ImpactDirection.Z += Ult_Angle;
			ImpactDirection = ImpactDirection.GetSafeNormal();

			// Log the impact direction for debugging
			UE_LOG(LogTemp, Log, TEXT("Impact Direction: %s"), *ImpactDirection.ToString());

			// Apply an impulse to the Diavolo character based on the impact direction and AirborneRate
			TestDiavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * Ult_Impulse, true);

			if (!TestDiavolo->GetbCanBeTestedMultipleTimes())
			{
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
				TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}
		}

	}

}

