// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPVolibear.generated.h"

/**
 * 
 */
UCLASS()
class START_API AOPVolibear : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPVolibear();

protected:
	virtual void BeginPlay() override;

	virtual void Passive() override;
	virtual void MeleeAttack() override;
	bool MeleeAttackTrace();
	virtual void Skill_1() override;

	virtual void Skill_2() override;

	virtual void Skill_3() override;
	
	void Skill_3_Lightningbolt();
	
	virtual void Skill_4() override;
	virtual void Ult() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USceneComponent> ShardSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float MeleeAttack_Impulse = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_1_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_Impulse = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_SlowAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_SlowDuration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Velocity = 0.f; // ��ų 4 �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Angle = 0.f; // ��ų 4 ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Distance = 0.f; // ��ų 4 �Ÿ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Impulse = 0.f; // �ñر� ��ݷ�

	FTimerHandle MeleeAttackCastTimer;
	FTimerHandle MeleeAttackComboCountTimer;
	int32 MeleeAttackComboCount = 0;
	float Skill_1_SlowDuration = 3.0f;
	
	FTimerHandle Skill_1_SpawnTimerHandle;

	bool bThunderingSmash = false; // Q 강화평타가 켜져있는지에 대한 bool
	bool bStormbringer = false; // 궁으로 변신한 상태인지

	FTimerHandle LightningboltSpawnTimer;
	FVector Skill_3_FinalLocation;

	FTimerHandle Skill_3_CastTimer;

	FVector Ult_FinalLocation;
	FTimerHandle Ult_StopTimer;

public:
	void ApplySkill_1_Effect(AOPChampion* SourceChampion, AOPDiavolo* OtherChampion);

	FORCEINLINE void ResetMeleeAttackComboCount() { MeleeAttackComboCount = 0; }
};
