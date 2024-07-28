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

	AOPDiavolo* Skill_2_Trace();

	virtual void Skill_3() override;
	
	void Skill_3_Lightningbolt();
	
	virtual void Skill_4() override;
	virtual void Ult() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void CreateMarkerOnTarget(AOPDiavolo* Target);

	void RemoveMarkerOnTarget(AOPDiavolo* Target);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Gameplay", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USceneComponent> ShardSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeAttack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float MeleeAttack_Impulse = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_1_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_Impulse = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_SlowAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_SlowDuration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Velocity_XY = 0.f; // ��ų 4 �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Velocity_Z = 0.f; // ��ų 4 �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	FVector Ult_Velocity_Parabola = FVector::ZeroVector; // ��ų 4 �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Angle = 0.f; // ��ų 4 ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Distance = 0.f; // ��ų 4 �Ÿ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Ult_Impulse = 0.f; // �ñر� ��ݷ�

	FTimerHandle MeleeAttackCastTimerHandle;
	FTimerHandle MeleeAttackComboCountTimerHandle;
	int32 MeleeAttackComboCount = 0;
	float Skill_1_SlowDuration = 3.0f;
	
	FTimerHandle Skill_1_SpawnTimerHandle;
	FTimerHandle Skill_1_RestTimer;

	FTimerHandle Skill_2_SpawnTimerHandle;
	FTimerHandle Skill_2_RestTimer;

	bool bThunderingSmash = false; // Q 강화평타가 켜져있는지에 대한 bool
	bool bStormbringer = false; // 궁으로 변신한 상태인지
	

	TObjectPtr<class AOPVolibearLightningbolt> Lightningbolt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPVolibearLightningbolt> LightningboltClass;
	
	FTimerHandle LightningboltSpawnTimerHandle;
	FVector Skill_3_FinalLocation;

	FTimerHandle Skill_3_CastTimerHandle;

	FVector Ult_FinalLocation;
	FTimerHandle Ult_StopTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	bool bUlt_ActAsProjectile = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	bool bUlt_ActAsGame = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	bool bUlt_ActAsParabola = false;
	//볼리베어 w스킬 갈퀴 마커 메쉬 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MarkerMesh;

public:
	FORCEINLINE void ResetMeleeAttackComboCount() { MeleeAttackComboCount = 0; }
};
