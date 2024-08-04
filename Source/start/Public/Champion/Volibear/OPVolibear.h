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

	/***************************************************************************** 패시브 *****************************************************************************/
	
	virtual void Passive() override;

	/****************************************************************************** 평타 ******************************************************************************/
	
	virtual void BasicAttack() override;
	void ResetMeleeAttackComboCount() { BasicAttackComboCount = 0; }
	bool MeleeAttackTrace();

	/***************************************************************************** 스킬 1 *****************************************************************************/

	virtual void Q() override;

	/***************************************************************************** 스킬 2 *****************************************************************************/

	virtual void W() override;

	AOPDiavolo* Skill_2_Trace();

	void CreateMarkerOnTarget(AOPDiavolo* Target);
	void RemoveMarkerOnTarget(AOPDiavolo* Target);

	/***************************************************************************** 스킬 3 *****************************************************************************/
	
	virtual void E() override;
	
	void Skill_3_Lightningbolt();

	/***************************************************************************** 궁극기 *****************************************************************************/
	virtual void R() override;

	/****************************************************************** Hit 이벤트 발생 시 호출할 함수 ******************************************************************/
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	/************************************************************** 투사체 움직임 구현을 위한 무브먼트 컴포넌트 **************************************************************/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	/****************************************************************************** 평타 ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeAttack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float MeleeAttack_Impulse = 0.f;

	FTimerHandle BasicAttackCastTimerHandle;
	FTimerHandle BasicAttackComboCountTimerHandle;
	int32 BasicAttackComboCount = 0;

	/***************************************************************************** 스킬 1 *****************************************************************************/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_1_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_1_SlowDuration = 3.0f;

	bool bThunderingSmash = false; // Q 강화평타가 켜져있는지에 대한 bool

	FTimerHandle Skill_1_SpawnTimerHandle;
	FTimerHandle Skill_1_RestTimer;
	
	/***************************************************************************** 스킬 2 *****************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_2_Impulse = 0.f;

	//볼리베어 w스킬 갈퀴 마커 메쉬 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 2 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MarkerMesh;
	
	FTimerHandle Skill_2_SpawnTimerHandle;
	FTimerHandle Skill_2_RestTimer;

	/***************************************************************************** 스킬 3 *****************************************************************************/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_SlowAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Skill_3_SlowDuration = 0.f;

	TObjectPtr<class AOPVolibearLightningbolt> Lightningbolt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPVolibearLightningbolt> LightningboltClass;

	FVector Skill_3_FinalLocation;
	
	FTimerHandle LightningboltSpawnTimerHandle;
	FTimerHandle Skill_3_CastTimerHandle;

	/***************************************************************************** 궁극기 *****************************************************************************/

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
	
	bool bStormbringer = false; // 궁으로 변신한 상태인지
	
	FVector Ult_FinalLocation;
	FTimerHandle Ult_StopTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	bool bUlt_ActAsProjectile = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	bool bUlt_ActAsGame = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	bool bUlt_ActAsParabola = false;
};
