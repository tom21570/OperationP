// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPYasuo.generated.h"

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
	virtual void Skill_2() override;
	virtual void Skill_3() override;
	virtual void Skill_4() override;
	virtual void Ult() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	int32 MeleeAttackComboCount = 0;
	int32 Skill_1_Stack = 0;
	FTimerHandle Skill_1_CastTimer;
	FTimerHandle Skill_1_StackTimer;
	FTimerHandle MeleeAttackComboCountTimer;

	FORCEINLINE void ResetMeleeAttackComboCount() { MeleeAttackComboCount = 0; }
	
};
