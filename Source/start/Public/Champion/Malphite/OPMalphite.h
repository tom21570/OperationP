// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPMalphite.generated.h"

class AOPMalphiteShardOfTheEarth;

/**
 * 
 */
UCLASS()
class START_API AOPMalphite : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPMalphite();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	/*************************************************************************** Passive ***************************************************************************/

	virtual void Passive() override;
	
	/************************************************************************ Basic Attack ************************************************************************/

	virtual void BasicAttack() override;
	void BasicAttackTrace();
	void BasicAttackTrace_W();
	
	/****************************************************************************** Q ******************************************************************************/

	virtual void Q() override;
	void Q_ShardOfTheEarth();
	
	/****************************************************************************** W ******************************************************************************/

	virtual void W() override;
	
	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;
	
	UFUNCTION()
	void E_GroundSlam();
	
	/****************************************************************************** R ******************************************************************************/

	virtual void R() override;
	void R_Trace();

	UFUNCTION()
	void R_OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	/*************************************************************************** Passive ***************************************************************************/

	/************************************************************************ Basic Attack ************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Strength_W = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Radius = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Radius_W = 600.f;

	FTimerHandle BasicAttack_Trace_TimerHandle;
	
	/****************************************************************************** Q ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USceneComponent> Q_ShardSpawnLocation;

	float Q_SlowDuration = 3.0f;
	FTimerHandle Q_Spawn_TimerHandle;

	TObjectPtr<AOPMalphiteShardOfTheEarth> Q_ShardOfTheEarthStorage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPMalphiteShardOfTheEarth> Q_ShardOfTheEarthClass;
	
	/****************************************************************************** W ******************************************************************************/

	bool bW_ThunderClapOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USceneComponent> W_ClapPoint;

	/****************************************************************************** E ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Angle = 0.f;

	FTimerHandle E_Cast_TimerHandle;
	
	/****************************************************************************** R ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Strength = 0.f;

	FVector R_FinalLocation; // 궁을 사용해서 도착할 지점
	FTimerHandle R_Stop_TimerHandle; // 궁 시전을 멈추기 위한 타이머핸들

	bool bR_IsCasting = false; // 궁을 시전 중이라면 true인 불리언 값
	
	// 말파는 디아볼로에서 피직스 시뮬레이트를 켜 놔야 q랑 궁에서 영상처럼 작동합니다
	
	/**************************************************************** Champion Combat Interactions ****************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float HP = 20000000.f;

public:
	float GetMalphite_HP();
	void SetMalphite_HP(float hp);
	
	void ApplySkill_1_Effect(AOPChampion* SourceChampion, AOPDiavolo* OtherChampion);

	void SetMalphite_HP_Damaged(float SourceChampionsForce, AOPMalphite* OtherChampion);
};
