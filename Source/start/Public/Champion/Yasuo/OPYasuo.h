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
	virtual void MeleeAttack() override;
	bool MeleeAttackTrace(); // 평타 발동 시 트레이스하는 함수
	virtual void Skill_1() override;
	bool Skill_1_Trace(); // 강철폭풍 발동 시 트레이스하는 함수
	void Skill_1_WhirlWind(); // 강철폭풍 3타에서 회오리 날리기
	virtual void Skill_2() override;
	void Skill_2_WindWall();
	virtual void Skill_3() override;
	virtual void Skill_4() override;
	virtual void Ult() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent; // 야스오 E 구현을 위해 야스오에 발사체 움직임을 추가해줌

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float MeleeAttack_Impulse = 0.f; // 평타 충격량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_1_Impulse = 0.f; // 스킬 1 충격량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Impulse = 0.f; // 스킬 3 충격량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Impulse = 0.f; // 궁극기 충격량
	
	
	int32 MeleeAttackComboCount = 0; // 평타 연속 사용 시 4번의 연결된 동작을 위한 콤보카운트
	int32 Skill_1_Stack = 0; // 강철폭풍 스택
	FTimerHandle Skill_1_CastTimer; // 강철폭풍 트레이스 시전시간을 위한 타이머
	FTimerHandle Skill_1_StackTimer; // 강철폭풍 스택 유지를 위한 타이머
	FTimerHandle MeleeAttackComboCountTimer; // 평타 연결된 동작 타이머

	TObjectPtr<AOPYasuoWhirlWind> WhirlWind; // 강철폭풍 3타 회오리를 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPYasuoWhirlWind> WhirlWindClass; // 위 변수에 담아서 실제로 날릴 회오리

	FTimerHandle WhirlWindSpawnTimer; // 회오리 딜레이 설정을 위한 타이머

	AOPYasuoWindWall* WindWall; // 바랑장벽을 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPYasuoWindWall> WindWallClass; // 바람장벽



	FORCEINLINE void ResetMeleeAttackComboCount() { MeleeAttackComboCount = 0; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	USoundCue* Skill_1_Hit_SFX; // 디아볼로가 강철폭풍 에 피격시 재생할 사운드 이펙트 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	USoundCue* Skill_1_Charged_SFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Attack", meta = (AllowPrivateAccess = "true"))
	USoundCue* MeleeAttack_Hit_SFX;
	
};
