// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPVolibear.generated.h"

class UOPVolibearAnimInstance;
class UNiagaraComponent;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class START_API AOPVolibear : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPVolibear();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	/***************************************************************************** 패시브 *****************************************************************************/
	
	virtual void Passive() override;

	/****************************************************************************** 평타 ******************************************************************************/
	
	virtual void BasicAttack() override;
	void BasicAttack_ResetComboCount() { BasicAttack_ComboCount = 0; }
	void BasicAttack_Trace();
	void BasicAttack_Trace_Q();

	/***************************************************************************** Q *****************************************************************************/

	virtual void Q() override;

	/***************************************************************************** W *****************************************************************************/

	virtual void W() override;
	void W_Trace();
	bool W_TraceForMaul();

	/***************************************************************************** E *****************************************************************************/
	
	virtual void E() override;
	void E_Lightningbolt();
	void E_Lightningbolt_Trace();

	/***************************************************************************** R *****************************************************************************/
	virtual void R() override;
	void R_OnLanding();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<USkeletalMeshComponent> ThousandPiercedBearMesh;

	TObjectPtr<UOPVolibearAnimInstance> ThousandPiercedBearAnimInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dismemberment", meta = (AllowPrivateAccess = "true"))
	float DismemberStrength = 0.f;
	/************************************************************** 투사체 움직임 구현을 위한 무브먼트 컴포넌트 **************************************************************/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	/****************************************************************************** 평타 ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Width = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	bool bBasicAttack_DrawDebugTrace = false;

	FTimerHandle BasicAttack_Cast_TimerHandle;
	FTimerHandle BasicAttack_ComboCount_TimerHandle;
	int32 BasicAttack_ComboCount = 0;

	/***************************************************************************** Q *****************************************************************************/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_Strength_XY = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_Strength_Z = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Q_MaintainTime = 4.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Q_WalkSpeed = 500.f;

	bool bQ_ThunderingSmash = false; // Q 강화평타가 켜져있는지에 대한 bool

	FTimerHandle Q_End_TimerHandle;
	
	/***************************************************************************** 스킬 2 *****************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float W_Range = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float W_Width = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	bool bW_DrawDebugTrace = false;
	
	FTimerHandle W_Cast_TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> W_BloodNiagara;

	/***************************************************************************** 스킬 3 *****************************************************************************/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_StrengthAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float E_Radius = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	bool bE_DrawDebugTrace = false;

	TObjectPtr<class AOPVolibearLightningbolt> E_LightningboltStorage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPVolibearLightningbolt> E_LightningboltClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> E_LightningShield_NiagaraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> E_LightningMarkNiagara;

	FVector E_FinalLocation;
	
	FTimerHandle E_LightningboltSpawn_TimerHandle;
	FTimerHandle E_Cast_TimerHandle;
	FTimerHandle E_NiagaraSpawn_TimerHandle;

	/***************************************************************************** 궁극기 *****************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Velocity_XY = 0.f; // ��ų 4 �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Velocity_Z = 0.f; // ��ų 4 �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_LandingStrength = 0.f; // �ñر� ��ݷ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float R_LandingRadius = 700.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_LandingStrengthAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float R_SizeIncreaseIndex = 1.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float R_MaintainTime = 1.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	bool bR_DrawDebugTrace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> R_LightningEffect;

	FVector R_OriginalSizeIndex;
	bool bR_Stormbringer = false; // 궁으로 변신한 상태인지
	bool bR_IsJumping = false;
	
	FVector R_FinalLocation;
	FTimerHandle R_End_TimerHandle;

	FTimerHandle SkeletonMontagePause_TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> BasicAttack_AnimMontage_Stormbringer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> Q_AnimMontage_Stormbringer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> W_AnimMontage_Stormbringer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> E_AnimMontage_Stormbringer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> R_AnimMontage_Stormbringer;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	// FVector Ult_Velocity_Parabola = FVector::ZeroVector; // ��ų 4 �ӵ�
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	// float Ult_Angle = 0.f; // ��ų 4 ����
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	// float Ult_Distance = 0.f; // ��ų 4 �Ÿ�
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	// bool bUlt_ActAsProjectile = true;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	// bool bUlt_ActAsGame = false;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
	// bool bUlt_ActAsParabola = false;

public:
	FORCEINLINE bool GetbQThunderingSmash() const { return bQ_ThunderingSmash; }
};
