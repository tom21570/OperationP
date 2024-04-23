// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/Yasuo/OPYasuo.h"
#include "Animation/OPAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/OPPlayerController.h"

AOPYasuo::AOPYasuo()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
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

	if(!GetbMeleeAttack()) return;
	if(!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if(MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	check(GetChampionAnimInstance());
	check(GetMeleeAttackAnimMontage());

	int32 Section = FMath::RandRange(0, 2);
	switch (MeleeAttackComboCount)
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

	SetbMeleeAttack_False();
	GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPYasuo::SetbMeleeAttack_True, GetMeleeAttackCooltime(), false);
}

void AOPYasuo::Skill_1()
{
	Super::Skill_1();
	
	if(!GetbSkill_1()) return;
	if(!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if(MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	check(GetChampionAnimInstance());
	check(GetSkill_1_AnimMontage());

	if(Skill_1_Stack == 2)
	{
		GetWorldTimerManager().ClearTimer(Skill_1_StackTimer);
		Skill_1_Stack = 0;
		GetChampionAnimInstance()->Montage_Play(GetSkill_1_AnimMontage(), 1.0f);
		GetChampionAnimInstance()->Montage_JumpToSection(FName("Wind"), GetSkill_1_AnimMontage());
	}

	else
	{
		int32 Section = FMath::RandRange(0, 1);
		switch (Section)
		{
		case 0:
			GetWorldTimerManager().SetTimer(Skill_1_CastTimer, FTimerDelegate::CreateLambda([&]
			{
				Skill_1_Trace();
			}), 0.2f, false);
			GetChampionAnimInstance()->Montage_Play(GetSkill_1_AnimMontage(), 1.0f);
			GetChampionAnimInstance()->Montage_JumpToSection(FName("1"), GetSkill_1_AnimMontage());
			break;

		case 1:
			GetWorldTimerManager().SetTimer(Skill_1_CastTimer, FTimerDelegate::CreateLambda([&]
			{
				Skill_1_Trace();
			}), 0.2f, false);
			GetChampionAnimInstance()->Montage_Play(GetSkill_1_AnimMontage(), 1.0f);
			GetChampionAnimInstance()->Montage_JumpToSection(FName("2"), GetSkill_1_AnimMontage());
			break;

		default:
			;
		}
	}
	


	SetbSkill_1_False();
	GetWorldTimerManager().SetTimer(Skill_1_CooltimeTimer, this, &AOPYasuo::SetbSkill_1_True, GetSkill_1_Cooltime(), false);

}

void AOPYasuo::Skill_1_Trace()
{
		TArray<FHitResult> HitResults;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);

		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 450.f, 40.f,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);
	
		if(HitResults.Num() > 0 && Skill_1_Stack < 2)
		{
			Skill_1_Stack++;
			GetWorldTimerManager().ClearTimer(Skill_1_StackTimer);
			GetWorldTimerManager().SetTimer(Skill_1_StackTimer, FTimerDelegate::CreateLambda([&]
			{
				Skill_1_Stack = 0;
				UE_LOG(LogTemp, Warning, TEXT("Stack Reset"));
			}), 6.f, false);
		}
}

void AOPYasuo::Skill_2()
{
	Super::Skill_2();
	
	if(!GetbSkill_2()) return;
	if(!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if(MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	check(GetChampionAnimInstance());
	check(GetSkill_2_AnimMontage());
	
	GetChampionAnimInstance()->Montage_Play(GetSkill_2_AnimMontage(), 1.0f);

	SetbSkill_2_False();
	GetWorldTimerManager().SetTimer(Skill_2_CooltimeTimer, this, &AOPYasuo::SetbSkill_2_True, GetSkill_2_Cooltime(), false);

}

void AOPYasuo::Skill_3()
{
	Super::Skill_3();
	
	if (!GetbSkill_3()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	
	AOPChampion* Enemy = Cast<AOPChampion>(MouseCursorHit.GetActor());
	if (Enemy == nullptr) return;

	int32 Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
	FVector Skill_3_FinalLocation;
	FTimerHandle Skill_3_EndTimer;

	if (Distance <= 475.f)
	{
		FVector Skill3Vector = MouseCursorHit.Location - GetActorLocation();
		Skill3Vector.Normalize();
		Skill3Vector *= 625.f;
		Skill3Vector.Z = 0.f;
		Skill_3_FinalLocation = GetActorLocation() + Skill3Vector;
		TurnCharacterToLocation(Enemy->GetActorLocation());
		ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(750.f, 0.f, 0.f));
		SetActorEnableCollision(false);
	}

	GetWorldTimerManager().SetTimer(Skill_3_EndTimer, FTimerDelegate::CreateLambda([&]
	{
		ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(0.f, 0.f, 0.f));
		SetActorEnableCollision(true);
	}), 475.f/750.f, false);
	
	check(GetChampionAnimInstance());
	check(GetSkill_3_AnimMontage());
	
	GetChampionAnimInstance()->Montage_Play(GetSkill_3_AnimMontage(), 1.0f);
	
	SetbSkill_3_False();
	GetWorldTimerManager().SetTimer(Skill_3_CooltimeTimer, this, &AOPYasuo::SetbSkill_3_True, GetSkill_3_Cooltime(), false);

}

void AOPYasuo::Skill_4()
{
	Super::Skill_4();
}

void AOPYasuo::Ult()
{
	Super::Ult();
	
	if(!GetbUlt()) return;
	if(!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	
	AOPChampion* Enemy = Cast<AOPChampion>(MouseCursorHit.GetActor());
	if (Enemy == nullptr) return;

	int32 Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());

	if (Distance <= 1400.f)
	{
		SetActorLocation(Enemy->GetActorLocation());
		// GetCharacterMovement()->AddImpulse(FVector(0.f, 0.f, 1000.f));
		GetCharacterMovement()->GravityScale = 0.f;
		// Enemy->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

	FTimerHandle Ult_End_Timer;
	GetWorldTimerManager().SetTimer(Ult_End_Timer, FTimerDelegate::CreateLambda([&]
	{
		GetCharacterMovement()->GravityScale = 1.f;
		// GetCharacterMovement()->AddImpulse(FVector(0.f, 0.f, -1000.f));
		// Enemy->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	}), 1.f, false);

	check(GetChampionAnimInstance());
	check(GetUlt_AnimMontage());

	GetChampionAnimInstance()->Montage_Play(GetUlt_AnimMontage(), 1.0f);
	
	SetbUlt_False();
	GetWorldTimerManager().SetTimer(Ult_CooltimeTimer, this, &AOPYasuo::SetbUlt_True, GetUlt_Cooltime(), false);

}
