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
#include "Champion/Malphite/OPMalphiteShardOfTheEarth.h"//\������Ʈ �������� ��ų�� ��ü

AOPMalphite::AOPMalphite()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // ����ü ������ �����Ϳ� ���� �Ҵ�

	ShardSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Shard Spawn Location"));
	ShardSpawnLocation->SetupAttachment(GetRootComponent());
}

void AOPMalphite::BeginPlay()
{
	Super::BeginPlay();
}

void AOPMalphite::Passive()
{
	Super::Passive();
}

void AOPMalphite::MeleeAttack() //��Ÿ
{
	Super::MeleeAttack();

	if (!bMeleeAttack) return;
	if (!OPPlayerController) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToCursor(MouseCursorHit);

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]
	{
			MeleeAttackTrace();
	}), 0.25f, false);

	if (ChampionAnimInstance && MeleeAttackAnimMontage)
	{
		if (bThunderClapOn == true)
		{
			ChampionAnimInstance->Montage_Play(MeleeAttackAnimMontage, 1.f);
			ChampionAnimInstance->Montage_JumpToSection(FName("clap"), MeleeAttackAnimMontage);
			bThunderClapOn = false;
		}
		else
		{
			ChampionAnimInstance->Montage_Play(MeleeAttackAnimMontage, 1.f);
		}
		
	}
	
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

void AOPMalphite::Skill_1() //������ �ĵ� (Seismic Shard): ����: ������Ʈ�� ������ ������ ������ ���� ���� ���ظ� ������ �̵� �ӵ��� ��Ĩ�ϴ�.
{
	if (!bSkill_1) return;
	if (!OPPlayerController) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	
	if (!MouseCursorHit.bBlockingHit) return;
	TurnCharacterToLocation(MouseCursorHit.Location);
	
	UE_LOG(LogTemp, Log, TEXT("Skill_1_ShardOfTheEarth"));
	ChampionAnimInstance->Montage_Play(Skill_1_AnimMontage, 1.0f);

	SetbSkill_1_False();
	GetWorldTimerManager().SetTimer(ShardOfTheEarthSpawnTimer, this, &AOPMalphite::Skill_1_ShardOfTheEarth, 0.25f, false);
	GetWorldTimerManager().SetTimer(Skill_1_CooltimeTimer, this, &AOPMalphite::SetbSkill_1_True, GetSkill_1_Cooltime(), false);
}

void AOPMalphite::ApplySkill_1_Effect(AOPChampion* SourceChampion, AOPDiavolo* OtherChampion)
{
	float OriginalSpeed = OtherChampion->GetCharacterMovement()->MaxWalkSpeed;

	// ������ �̵� �ӵ��� 20% ���ҽ�ŵ�ϴ�.
	float SlowedSpeed = OriginalSpeed * 0.8f;
	OtherChampion->GetCharacterMovement()->MaxWalkSpeed = SlowedSpeed;

	// ��ų�� �ߵ��� ĳ���͸� ã���ϴ�.
	if (AOPMalphite* SourceCharacter = Cast<AOPMalphite>(SourceChampion))
	{
		// ���ΰ��� �̵� �ӵ��� ���ҵ� �ӵ���ŭ ������ŵ�ϴ�.
		float SpeedIncrease = OriginalSpeed - SlowedSpeed;
		SourceCharacter->GetCharacterMovement()->MaxWalkSpeed += SpeedIncrease;
		// ���� �ð� �Ŀ� ���� �ӵ��� �ǵ����� Ÿ�̸Ӹ� �����մϴ�.
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, [this, SourceCharacter, OriginalSpeed, SpeedIncrease]()
		{
			// ������ �̵� �ӵ��� ������� �ǵ����ϴ�.
			GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
			
			// ���ΰ��� �̵� �ӵ��� ������� �ǵ����ϴ�.
			SourceCharacter->GetCharacterMovement()->MaxWalkSpeed -= SpeedIncrease;
		}, Skill_1_SlowDuration, false);
	}
}

void AOPMalphite::Skill_1_ShardOfTheEarth()
{
	if (ShardOfTheEarthClass == nullptr) return;

	ShardOfTheEarth = GetWorld()->SpawnActor<AOPMalphiteShardOfTheEarth>(ShardOfTheEarthClass, ShardSpawnLocation->GetComponentLocation(), GetActorRotation());
	ShardOfTheEarth->SetOwner(this);
}

void AOPMalphite::Skill_2() //õ���� ���� (Thunderclap): ����: ������Ʈ�� ���� �⺻ ������ �߰� ���� ���ظ� ������, �ֺ��� ���鿡�� �߰� ���ظ� �ݴϴ�. ���ָ�// �߰� �ʿ��� ��, ������Ʈ �� ����Ʈ, ���ָ� ����Ʈ
{
	Super::Skill_2();

	bThunderClapOn = true;

	if (ChampionAnimInstance && Skill_2_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Skill_2_AnimMontage, 1.f);
	}

}

void AOPMalphite::Skill_3() //���� ��Ÿ (Ground Slam): ����: ������Ʈ�� ������ ��Ÿ�Ͽ� ������ ���鿡�� ���� ���ظ� ������ ���� �ӵ��� ���ҽ�ŵ�ϴ�. ���� �Լ� Skill_3_GroundSlam, Skill_3_ApplySlowAttackEffect
{
	Super::Skill_3();

	if (!bSkill_3) return;
	if (!OPPlayerController) return;

	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);

	if (MouseCursorHit.bBlockingHit)
	{
		TurnCharacterToCursor(MouseCursorHit);
	}

	GetWorldTimerManager().SetTimer(Skill_3_CastTimer, FTimerDelegate::CreateLambda([&]
	{
		Skill_3_GroundSlam();
	}), 0.25f, false);

	if (ChampionAnimInstance && Skill_3_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Skill_3_AnimMontage, 1.f);
		ChampionAnimInstance->Montage_JumpToSection(FName("GroundSlam"), Skill_3_AnimMontage);
	}

	SetbSkill_3_False();
	GetWorldTimerManager().SetTimer(Skill_3_CooltimeTimer, this, &AOPMalphite::SetbSkill_3_True, GetSkill_3_Cooltime(), false);
}


void AOPMalphite::Skill_3_GroundSlam()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * Skill_3_Radius, Skill_3_Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResults, true);

	for (auto& HitActor : HitResults)
	{
		if (AOPDiavolo* HitDiavolo = Cast<AOPDiavolo>(HitActor.GetActor()))
		{
			if (HitDiavolo)
			{
				FVector MalphiteLocation = FVector(GetActorLocation().X, GetActorLocation().Y, 0.f);
				FVector ImpulseDirection = (HitDiavolo->GetActorLocation() - MalphiteLocation).GetSafeNormal();
				HitDiavolo->SetbIsDamagedTrue();
				HitDiavolo->PlayDiavoloRandomDeadMontage();
				HitDiavolo->LaunchCharacter(ImpulseDirection * Skill_3_Impulse, true, true);
				// HitDiavolo->GetCharacterMovement()->AddImpulse(ImpulseDirection * Skill_3_Impulse, true);
				HitDiavolo->ApplySlowAttackEffect(Skill_3_SlowAmount, Skill_3_SlowDuration); //��ƺ��ο� ���ݸ���� �������� �Լ� �����ʿ�
			}
		}
	}
}

void AOPMalphite::Skill_4() 
{
}

void AOPMalphite::Ult() //���� �� ���� �� (Unstoppable Force): ����: ������Ʈ�� ������ ��ġ�� �����Ͽ� ���� �� �ֺ��� ���鿡�� ���� ���ظ� ������ �˹��ŵ�ϴ�. //������Ʈ�� ������ ���ϴµ�, ��������Ʈ�� ĸ��������Ʈ�� �۾Ƽ� �׷�����..
{
	Super::Ult();

	if (!bUlt) return;
	if (!OPPlayerController) return;
	
	OPPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;

	if (ChampionAnimInstance && Ult_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(Ult_AnimMontage, 1.0f);
	}
	
	Ult_FinalLocation = MouseCursorHit.Location;
	TurnCharacterToLocation(Ult_FinalLocation);
	FVector LaunchVector = Ult_FinalLocation - GetActorLocation();
	LaunchVector.Normalize();
	LaunchVector.Z = 0.f;
	const float FinalDistance = (Ult_FinalLocation - GetActorLocation()).Length();
	
	LaunchCharacter(LaunchVector * Ult_Velocity, true, true);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AOPMalphite::OnProjectileHit);
	
	GetWorldTimerManager().SetTimer(Ult_StopTimer, FTimerDelegate::CreateLambda([&]
	{
		GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &AOPMalphite::OnProjectileHit);
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Cyan, FString::Printf(TEXT("Hit Done")));
			
	}), (Ult_FinalLocation / Ult_Velocity).Size(), false);
	
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), FinalDistance);
	
	// ProjectileMovementComponent->Velocity = GetActorForwardVector() * Ult_Velocity;
	
	SetbUlt_False();
	GetWorldTimerManager().SetTimer(Ult_CooltimeTimer, this, &AOPMalphite::SetbUlt_True, GetUlt_Cooltime(), false);
}

// Collision event handler
void AOPMalphite::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && GetOwner() != nullptr && OtherActor != GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnProjectileHit Active 1"));
		UE_LOG(LogTemp, Warning, TEXT("HitComponent: %s / OtherActor: %s / OtherComp: %s / Hit: %s"),
			*HitComponent->GetName(),
			*OtherActor->GetName(),
			*OtherComp->GetName(),
			*Hit.ToString());
		UE_LOG(LogTemp, Warning, TEXT("OnProjectileHit Active 2"));

		if (AOPDiavolo* Diavolo = Cast<AOPDiavolo>(OtherActor))
		{            // Calculate the direction of the impulse
			FVector ImpactDirection = (Diavolo->GetActorLocation() - Hit.ImpactPoint).GetSafeNormal();

			// Add an upward component to the impact direction
			ImpactDirection.Z += Ult_Angle;
			ImpactDirection = ImpactDirection.GetSafeNormal();

			// Log the impact direction for debugging
			UE_LOG(LogTemp, Log, TEXT("Impact Direction: %s"), *ImpactDirection.ToString());

			// Apply an impulse to the Diavolo character based on the impact direction and AirborneRate
			Diavolo->GetCharacterMovement()->AddImpulse(ImpactDirection * Ult_Impulse, true);

			if (!Diavolo->GetbCanBeTestedMultipleTimes())
			{
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
				Diavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
				// TestDiavolo->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}
		}

	}

}
