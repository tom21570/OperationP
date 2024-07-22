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

AOPTristana::AOPTristana()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component")); // 투사체 움직임 포인터에 동적 할당
}

void AOPTristana::BeginPlay()
{
	Super::BeginPlay();
}

void AOPTristana::Passive()
{

}

void AOPTristana::MeleeAttack()
{
	Super::MeleeAttack();

	if (!GetbLongDistanceAttack()) return;
	if (!GetOPPlayerController()) return;

	GetOPPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, MouseCursorHit);
	if (!MouseCursorHit.bBlockingHit) return;
	UE_LOG(LogTemp, Warning, TEXT("cursor Hit"));
	TurnCharacterToCursor(MouseCursorHit);

	TestDiavolo = Cast<AOPDiavolo>(MouseCursorHit.GetActor());
	if (TestDiavolo == nullptr) return;

	MeleeAttack_CannonBall();

	GetWorldTimerManager().SetTimer(CannonBallClassSpawnTimer, this, &AOPTristana::MeleeAttack_CannonBall, 0.25f, false);
	//GetWorldTimerManager().SetTimer(MeleeAttackCooltimeTimer, this, &AOPTristana::SetbMeleeAttack, GetMeleeAttackCooltime(), false);
	
	
	/* 범위 이동 후 공격은 추후 추가
	FVector Start = GetActorLocation();
	FVector TestDiavoloLocation = TestDiavolo->GetActorLocation();

	float DistanceToTarget = CalculateMinDistanceToActorEdge(Start, TestDiavoloLocation, LongDistanceAttack_Range);

	FTimerHandle LongDistanceAttack_EndTimer;


	if (DistanceToTarget <= LongDistanceAttack_Range)
	{
		// 공격 범위 내에 있으면 공격
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		GetChampionAnimInstance()->Montage_Play(LongDistanceAttackAnimMontage, 1.f);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, TestDiavoloLocation, ECC_Visibility, Params))
		{
			// 데미지 적용
			if (AActor* HitActor = HitResult.GetActor())
			{
				// 임시로 로그 출력
				UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetName());
				//디아볼로 쓰러지는 모션 혹은 공격당하는 모션 추가
			}
		}
	}
	else
	{
		// 공격 범위 밖에 있으면 이동
		FVector Direction = (TestDiavoloLocation - Start).GetSafeNormal();
		FVector MoveLocation = TestDiavoloLocation - Direction * LongDistanceAttack_Range;

		UE_LOG(LogTemp, Warning, TEXT("Moving to: %s"), *MoveLocation.ToString());
		MoveToLocation(MoveLocation);
	}
	*/
}

void AOPTristana::MeleeAttack_CannonBall()
{
	UE_LOG(LogTemp, Log, TEXT("MeleeAttack_CannonBall"));
	ChampionAnimInstance->Montage_Play(GetMeleeAttackAnimMontage(), 1.0f);

	if (CannonBallClass == nullptr) return;

	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f; // 캐릭터보다 살짝 앞에 스폰
	FRotator SpawnRotation = GetActorRotation();

	if (CannonBall = GetWorld()->SpawnActor<AOPTristanaCannonBall>(CannonBallClass, SpawnLocation, SpawnRotation))
	{
		CannonBall->SetOwner(this);
		FVector LaunchDirection = GetActorForwardVector();
	}
}


void AOPTristana::Skill_1()  //빠른 발사 (Rapid Fire) 효과: 일정 시간 동안 트리스타나의 공격 속도가 대폭 증가합니다.
{
}

void AOPTristana::Skill_2() //로켓 점프 (Rocket Jump) 효과: 트리스타나가 목표 지점으로 로켓을 이용해 뛰어올라, 착지 지점에서 범위 피해를 입히고 적들을 둔화시킵니다. 재사용: 로켓 점프는 적을 처치하거나 어시스트를 기록하면 재사용 대기 시간이 초기화됩니다.
{
}

void AOPTristana::Skill_3() //폭발 화약(Explosive Charge) 		효과: 패시브로, 트리스타나가 처치한 적이 폭발하여 주변 적들에게 피해를 입힙니다.활성화 : 트리스타나가 적에게 폭발물 패키지를 설치합니다.시간이 지나거나 트리스타나가 일정 횟수 공격을 가하면 폭발하여 큰 피해를 입힙니다.
{
}

void AOPTristana::Ult() //대구경 탄환 (Buster Shot)효과: 트리스타나가 강력한 탄환을 발사하여 대상에게 피해를 입히고, 대상을 멀리 밀어냅니다. 활성화 : 이 스킬은 적 챔피언을 밀어내는 데 효과적이며, 특히 위험한 적을 멀리 밀어내어 위기 상황에서 탈출하거나 적을 분리시키는 데 사용할 수 있습니다.
{
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




