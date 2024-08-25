// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPTristana.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;
class AOPTristanaCannonBall;
class AOPTristanaBusterShot;
class AOPTristanaExplosiveCharge;
/**
 * 
 */
UCLASS()
class START_API AOPTristana : public AOPChampion
{
	GENERATED_BODY()
	
public:
	AOPTristana();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
protected:
	/*************************************************************************** Passive ***************************************************************************/

	virtual void Passive() override;
	
	/************************************************************************ Basic Attack ************************************************************************/

	virtual void BasicAttack() override;
	void BasicAttack_CannonBall();
	
	/****************************************************************************** Q ******************************************************************************/

	virtual void Q() override;
	void Q_EndRapidFire();
	
	/****************************************************************************** W ******************************************************************************/

	virtual void W() override;
	void W_OnLanding();
	void W_Play_JumpAnimMontage();
	
	UFUNCTION()
	void W_OnOverlappingDiavolo(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;
	void E_UseExplosiveCharge(AOPDiavolo* Target);
	
	/****************************************************************************** R ******************************************************************************/

	virtual void R() override;
	void R_BusterShot();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;
	
	/*************************************************************************** Passive ***************************************************************************/

	/************************************************************************ Basic Attack ************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Speed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Angle = 0.f;
	
	TObjectPtr<AOPTristanaCannonBall> BasicAttack_CannonBallStorage; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaCannonBall> BasicAttack_CannonBallClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack", meta = (AllowPrivateAccess = "true"))
	const TObjectPtr<USkeletalMeshSocket> ProjectileSpawnSocket;

	FTimerHandle BasicAttack_CannonBallSpawn_TimerHandle; 
	
	float BasicAttack_DefaultAttackSpeed;
	
	/****************************************************************************** Q ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_RapidFireValue = 0.5f;
	
	FTimerHandle Q_TimerHandle; 
	bool bQ_IsActive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Q_Duration = 10.f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> Q_NiagaraComponent;

	/****************************************************************************** W ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_MaxJumpRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_Speed_XY = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_Speed_Z = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_LandingStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_LandingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_LandingStrengthAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> W_Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> W_ParticleSystem_JumpStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> W_NiagaraComponent_Landing;

	FTimerHandle W_Jump_TimerHandle;
	
	bool bIsWJumping;

	TArray<TObjectPtr<AOPDiavolo>> SteppedDiavolo;
	
	/****************************************************************************** E ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	AOPDiavolo* E_Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed = 2400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_SpawnAngle = 60.f;

	TObjectPtr<AOPTristanaExplosiveCharge> E_ExplosiveChargeStorage; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaExplosiveCharge> E_ExplosiveChargeClass; 
	
	AOPTristanaExplosiveCharge* E_CurrentExplosiveCharge;

	FTimerHandle E_Spawn_TimerHandle;
	
	/****************************************************************************** R ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Strength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Rebound = 5000.f;
	
	TObjectPtr<AOPTristanaBusterShot> R_BusterShotStorage; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaBusterShot> R_BusterShotClass; 

	FTimerHandle R_Spawn_TimerHandle; 
};
