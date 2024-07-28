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

protected:
	virtual void BeginPlay() override;

	virtual void Passive() override;
	virtual void MeleeAttack() override;
	bool MeleeAttackTrace(); // ��Ÿ �ߵ� �� Ʈ���̽��ϴ� �Լ�
	virtual void Skill_1() override;

	void Skill_1_ShardOfTheEarth();

	virtual void Skill_2() override;

	virtual void Skill_3() override;
	
	UFUNCTION()
	void Skill_3_GroundSlam();
	
	virtual void Skill_4() override;
	virtual void Ult() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent; //�߻�ü �������� �߰�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USceneComponent> ShardSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	float MeleeAttack_Impulse = 0.f;
	
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

	float Skill_1_SlowDuration = 3.0f;
	FTimerHandle Skill_1_SpawnTimerHandle;

	bool bThunderClapOn = false;
	
	TObjectPtr<AOPMalphiteShardOfTheEarth> ShardOfTheEarth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPMalphiteShardOfTheEarth> ShardOfTheEarthClass;

	FTimerHandle ShardOfTheEarthSpawnTimer;

	FTimerHandle Skill_3_CastTimer;

	FVector Ult_FinalLocation;
	FTimerHandle Ult_StopTimer;

public:
	void ApplySkill_1_Effect(AOPChampion* SourceChampion, AOPDiavolo* OtherChampion);
};
