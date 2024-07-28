// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPTristana.generated.h"

/**
 * 
 */
class AOPTristanaCannonBall;
class AOPTristanaBusterShot;
class AOPTristanaExplosiveCharge;
UCLASS()
class START_API AOPTristana : public AOPChampion
{
	GENERATED_BODY()
public:
	AOPTristana();

protected:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Passive() override;
	virtual void MeleeAttack() override; //원거리 기본공격
	void MeleeAttack_CannonBall();
	virtual void Skill_1() override; //빠른 발사 (Rapid Fire) 효과: 일정 시간 동안 트리스타나의 공격 속도가 대폭 증가합니다.
	void EndRapidFire();
	virtual void Skill_2() override; //로켓 점프 (Rocket Jump) 효과: 트리스타나가 목표 지점으로 로켓을 이용해 뛰어올라, 착지 지점에서 범위 피해를 입히고 적들을 둔화시킵니다. 재사용: 로켓 점프는 적을 처치하거나 어시스트를 기록하면 재사용 대기 시간이 초기화됩니다.
	void OnLanding();
	void PlaySkill_2_JumpAnimMontage();
	virtual void Skill_3() override; //폭발 화약(Explosive Charge) 		효과: 패시브로, 트리스타나가 처치한 적이 폭발하여 주변 적들에게 피해를 입힙니다.		활성화 : 트리스타나가 적에게 폭발물 패키지를 설치합니다.시간이 지나거나 트리스타나가 일정 횟수 공격을 가하면 폭발하여 큰 피해를 입힙니다.
	// Function to use Explosive Charge
	void UseExplosiveCharge(AOPDiavolo* Target);

	virtual void Ult() override; //대구경 탄환 (Buster Shot)효과: 트리스타나가 강력한 탄환을 발사하여 대상에게 피해를 입히고, 대상을 멀리 밀어냅니다. 활성화 : 이 스킬은 적 챔피언을 밀어내는 데 효과적이며, 특히 위험한 적을 멀리 밀어내어 위기 상황에서 탈출하거나 적을 분리시키는 데 사용할 수 있습니다.
	void Ult_BusterShot();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent; //발사체 움직임을 추가해줌

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float LongDistanceAttack_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float LongDistanceAttack_Range = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float LongDistanceAttack_Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_MaxJumpRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_ShortJumpRange = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_MiddleJumpRange = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_LongJumpRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_Velocity_XY = 0.f; // ??? 4 ???

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_Velocity_Z = 0.f; // ??? 4 ???

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_JumpStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_LandingDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_LandingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_SlowDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	AOPDiavolo* Skill_3_Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_TimeToExplode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_MaxThrowRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Impulse;


	TObjectPtr<AOPTristanaCannonBall> CannonBall; // 평타 캐논볼 담을 함수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeAttack", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaCannonBall> CannonBallClass; // 위 변수에 담아서 실제로 날릴 캐논볼

	FTimerHandle CannonBallClassSpawnTimer; // 캐논볼 딜레이 설정을 위한 타이머
	FTimerHandle Skill_2_JumpTimerHandle;
	FVector Skill_2_FinalLocation;
	FVector Skill_2_Vector_XY;

	TObjectPtr<AOPTristanaBusterShot> BusterShot; // 버스터샷 담을 함수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaBusterShot> BusterShotClass; // 위 변수에 담아서 실제로 날릴 버스터샷

	FTimerHandle BusterShotClassSpawnTimer; // 버스터샷 딜레이 설정을 위한 타이머


	TObjectPtr<AOPTristanaExplosiveCharge> ExplosiveCharge; // 폭발 화약 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaExplosiveCharge> ExplosiveChargeClass; // 위 변수에 담아서 실제로 날릴 폭발화약

	// Current active explosive charge
	AOPTristanaExplosiveCharge* CurrentExplosiveCharge;

	FTimerHandle ExplosiveChargeClassSpawnTimer; // 캐논볼 딜레이 설정을 위한 타이머



	//skill1 rapid fire
	float DefaultAttackSpeed;

	FTimerHandle RapidFireTimerHandle;

	bool bIsRapidFireActive = false;

	float RapidFireDuration;

	//skill rocket jump
	FVector TargetLocation;


	// Timer handle for explosion
	FTimerHandle ExplosionTimerHandle;
	FTimerHandle ExplosiveChargeSpawnTimerHandle;


	// Handle Landing

	//FVector MoveTargetLocation; //이동 관련 변수
	//bool bIsMoving; //이동 관련 변수

	//float CalculateMinDistanceToActorEdge(FVector ActorLocation, FVector TargetLocation, float Radius);
	//void MoveToLocation(FVector TargetLocation);
};


