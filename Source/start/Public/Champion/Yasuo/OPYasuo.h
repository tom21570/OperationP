// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPYasuo.generated.h"

class AOPYasuoWhirlWind;
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
	void MeleeAttackTrace();
	virtual void Skill_1() override;
	void Skill_1_Trace();
	void Skill_1_WhirlWind();
	virtual void Skill_2() override;
	virtual void Skill_3() override;
	virtual void Skill_4() override;
	virtual void Ult() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float MeleeAttack_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_1_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Impulse = 0.f;
	
	int32 MeleeAttackComboCount = 0;
	int32 Skill_1_Stack = 0;
	FTimerHandle Skill_1_CastTimer;
	FTimerHandle Skill_1_StackTimer;
	FTimerHandle MeleeAttackComboCountTimer;

	TObjectPtr<AOPYasuoWhirlWind> WhirlWind;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPYasuoWhirlWind> WhirlWindClass;

	FTimerHandle WhirlWindSpawnTimer;

	FORCEINLINE void ResetMeleeAttackComboCount() { MeleeAttackComboCount = 0; }
	
};
