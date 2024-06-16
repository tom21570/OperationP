// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPLeeSin.generated.h"

class AOPLeeSinSonicWave;
class AOPLeeSinDragonsRage;
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

	void Skill_Ult_DragonsRage();//용의 분노

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent; //  동적으로 추가되는 투사체

	int32 MeleeAttackComboCount = 0; // 평타 연속 사용 시 4번의 연결된 동작을 위한 콤보카운트
	FTimerHandle MeleeAttackComboCountTimer; // 평타 연결된 동작 타이머

	int32 Skill_1_Stack = 0; // Skill_1 스택  1: 소닉웨이브 -> 2: 이동기
	FTimerHandle Skill_1_CastTimer; // 이동기 시전시간을 위한 타이머
	FTimerHandle Skill_1_StackTimer; // 소닉웨이브 스택 유지 -> 이동기 위한 타이머

	int32 Skill_3_Stack = 0; // Skill_3 스택  1: 땅치기 -> 2: 걸린 유닛 이동속도 저하 
	FTimerHandle Skill_3_CastTimer; // 이동속도 저하  시전시간을 위한 타이머
	FTimerHandle Skill_3_StackTimer; // 땅치기 스택 유지 -> 이동속도 저하  시전시간 타이머

	TObjectPtr<AOPLeeSinSonicWave> SonicWave; // 음파를 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPLeeSinSonicWave> SonicWaveClass; // 위 변수에 담아서 실제로 날릴 음파

	FTimerHandle SonicWaveSpawnTimer; // 회오리 딜레이 설정을 위한 타이머
	FTimerHandle DragonsRageSpawnTimer; // 회오리 딜레이 설정을 위한 타이머

	TObjectPtr<AOPLeeSinDragonsRage> DragonsRage; // 용의분노를 담을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPLeeSinDragonsRage> DragonsRageClass; // 용의 분노가 실제로 담길 class


	FORCEINLINE void ResetMeleeAttackComboCount() { MeleeAttackComboCount = 0; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
