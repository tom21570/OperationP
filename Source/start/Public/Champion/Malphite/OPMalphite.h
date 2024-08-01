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
	virtual void BasicAttack() override;
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

	UFUNCTION()
	virtual void UltBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent; //�߻�ü �������� �߰�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Gameplay Methods", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USceneComponent> ShardSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Impulse = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_SlowAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_SlowDuration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Speed = 0.f; // ��ų 4 �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Angle = 0.f; // ��ų 4 ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Distance = 0.f; // ��ų 4 �Ÿ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Impulse = 0.f; // �ñر� ��ݷ�

	FTimerHandle BasicAttackMovementStopTimerHandle;

	float Skill_1_SlowDuration = 3.0f;
	FTimerHandle Skill_1_SpawnTimerHandle;

	bool bThunderClapOn = false;
	
	TObjectPtr<AOPMalphiteShardOfTheEarth> ShardOfTheEarth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPMalphiteShardOfTheEarth> ShardOfTheEarthClass;

	FTimerHandle ShardOfTheEarthSpawnTimer;

	FTimerHandle Skill_3_CastTimer;

	FVector Ult_FinalLocation;
	FTimerHandle Ult_StopTimer;

public:
	void ApplySkill_1_Effect(AOPChampion* SourceChampion, AOPDiavolo* OtherChampion);
};
