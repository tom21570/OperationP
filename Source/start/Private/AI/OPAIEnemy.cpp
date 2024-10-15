// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPAIEnemy.h"
#include "Animation/OPAnimInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Projectile/OPSkeletonKnightSpear.h"

AOPAIEnemy::AOPAIEnemy()
{
	SpearSpawnPoint = CreateDefaultSubobject<USceneComponent>("SpearSpawnPoint");
	SpearSpawnPoint->SetupAttachment(GetRootComponent());
}

void AOPAIEnemy::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f);

	if (Tree)
	{
	}
}

void AOPAIEnemy::Tick(float DeltaSeconds)
{
	if (!DamagedByChampion)
	{
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0.f);
	    DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

		if (DistanceToPlayer >= AttackableDistance)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Player->GetActorLocation());
		}
		else if (DistanceToPlayer < AttackableDistance)
		{
			TurnCharacterToLocation(Player->GetActorLocation());
			BasicAttack();
		}
	}
}

void AOPAIEnemy::BasicAttack()
{
	Super::BasicAttack();

	if (!bBasicAttack) return;

	GetWorldTimerManager().SetTimer(SpearThrow_TimerHandle, this, &AOPAIEnemy::ThrowSpear, 0.8f, false);
	
	if (ChampionAnimInstance && BasicAttack_AnimMontage)
	{
		ChampionAnimInstance->Montage_Play(BasicAttack_AnimMontage);
	}

	StopChampionMovement();
	GetWorldTimerManager().SetTimer(ResetMovementTimerHandle, this, &AOPAIEnemy::ResetChampionMovement, 1.9f, false);
	SetbBasicAttack_False();
	GetWorldTimerManager().SetTimer(BasicAttack_Cooldown_TimerHandle, this, &AOPAIEnemy::SetbBasicAttack_True, 1.9f, false);
}

// void AOPAIEnemy::SetupStimulusSource()
// {
// 	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("Stimulus");
// 	if (StimulusSource)
// 	{
// 		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
// 		StimulusSource->RegisterWithPerceptionSystem();
// 	}
// }

void AOPAIEnemy::ThrowSpear()
{
	if (SpearClass == nullptr) return;
	SpearStorage = GetWorld()->SpawnActor<AOPSkeletonKnightSpear>(SpearClass, SpearSpawnPoint->GetComponentLocation(), SpearSpawnPoint->GetComponentRotation());
	SpearStorage->SetOwner(this);
}

int AOPAIEnemy::MeleeAttack_AI_Implementation()
{
	if (BasicAttack_AnimMontage)
	{
		PlayAnimMontage(BasicAttack_AnimMontage);
	}
	return 0;
}

void AOPAIEnemy::BasicAttack_Public()
{
	BasicAttack();
}

void AOPAIEnemy::Die()
{
	// GetWorldTimerManager().SetTimer(Death_TimerHandle, FTimerDelegate::CreateLambda([&]
	// {
	// 	GetController()->UnPossess()
	// }), 0.2f, false);
}
