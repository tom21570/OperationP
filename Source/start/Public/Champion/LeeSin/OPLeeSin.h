// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPLeeSin.generated.h"

UCLASS()
class START_API AOPLeeSin : public AOPChampion
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOPLeeSin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Passive() override;
	virtual void MeleeAttack() override;

	virtual void Skill_1() override;

	void Skill_1_SonicWave();//소닉웨이브

	virtual void Skill_2() override;

	virtual void Skill_3() override;

	virtual void Ult() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SonicWave Projectile", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* SonicWave; // 소닉웨이브만 동적으로 추가되는 발사체

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dragon's Rage", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* DragonRage; // DragonRage만 동적으로 추가되는 발사체


	int32 MeleeAttackComboCount = 0; // 평타 연속 사용 시 4번의 연결된 동작을 위한 콤보카운트
	FTimerHandle MeleeAttackComboCountTimer; // 평타 연결된 동작 타이머



	FORCEINLINE void ResetMeleeAttackComboCount() { MeleeAttackComboCount = 0; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
