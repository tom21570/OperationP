// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPYasuo.generated.h"

class AOPYasuoWhirlWind;
class AOPYasuoWindWall;

/**
 * 
 */
UCLASS()
class START_API AOPYasuo : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPYasuo();

protected:
	virtual void BeginPlay() override;
	
	virtual void Passive() override;
	virtual void BasicAttack() override;
	
	UFUNCTION()
	bool MeleeAttackTrace(); // 평타 발동 시 트레이스하는 함수
	
	virtual void Q() override;
	void PlaySkill_1_OrdinaryAnimMontage();

	UFUNCTION()
	bool Skill_1_Trace(); // 강철폭풍 발동 시 트레이스하는 함수
	
	void Skill_1_WhirlWind(); // 강철폭풍 3타에서 회오리 날리기
	virtual void W() override;
	void Skill_2_WindWall();
	virtual void E() override;
	virtual void R() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent; // 야스오 E 구현을 위해 야스오에 발사체 움직임을 추가해줌

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Impulse = 0.f; // 평타 충격량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_1_Impulse = 0.f; // 스킬 1 충격량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Impulse = 0.f; // 스킬 3 충격량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_DistanceBetweenEnemy = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Speed = 0.f; // 스킬 3 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_CastTime = 0.f; // 발도술을 시전하는 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 4 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_4_Impulse = 0.f; // 스킬 4 충격량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 4 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_4_Velocity = 0.f; // 스킬 4 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 4 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_4_Angle = 0.f; // 스킬 4 각도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 4 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_4_Distance = 0.f; // 스킬 4 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Impulse = 0.f; // 궁극기 충격량
	
	int32 BasicAttackComboCount = 0; // 평타 연속 사용 시 4번의 연결된 동작을 위한 콤보카운트
	int32 Skill_1_Stack = 0; // 강철폭풍 스택
	FTimerHandle BasicAttackCastTimer; // 평타 시전을 위한 타이머
	FTimerHandle Skill_1_CastTimer; // 강철폭풍 트레이스 시전시간을 위한 타이머
	FTimerHandle Skill_1_StackTimer; // 강철폭풍 스택 유지를 위한 타이머

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Skill_1_Swift_AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	bool bSkill_1_CanSwift = false;

	FTimerHandle Skill_1_Swift_StartTimerHandle;
	FTimerHandle Skill_1_Swift_EndTimerHandle;
	
	FTimerHandle BasicAttackComboCountTimer; // 평타 연결된 동작 타이머
	FTimerHandle Skill_3_EndTimer;
	FVector MoveTargetLocation; //스킬4 이동 장소(디아볼로)
	FVector Skill_4_TargetDirection; //스킬4 방향 고정;
	FTimerHandle MoveTimerHandle; //이동 시간 타이머
	FTimerHandle Ult_End_Timer;
	
	TObjectPtr<AOPYasuoWhirlWind> WhirlWind; // 강철폭풍 3타 회오리를 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPYasuoWhirlWind> WhirlWindClass; // 위 변수에 담아서 실제로 날릴 회오리

	FTimerHandle WhirlWindSpawnTimer; // 회오리 딜레이 설정을 위한 타이머

	TObjectPtr<AOPYasuoWindWall> WindWall; // 바랑장벽을 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPYasuoWindWall> WindWallClass; // 바람장벽

	// 추가할 이벤트 핸들러 선언
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnDrawingSword(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TArray<TObjectPtr<AOPChampion>> SlicedChampions;

	
	FORCEINLINE void ResetMeleeAttackComboCount() { BasicAttackComboCount = 0; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> Skill_1_Hit_SFX; // 디아볼로가 강철폭풍 에 피격시 재생할 사운드 이펙트 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> Skill_1_Charged_SFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> BasicAttack_Hit_SFX;
};
