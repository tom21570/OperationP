// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPTristana.generated.h"

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

protected:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	
	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;
	void E_UseExplosiveCharge(AOPDiavolo* Target);
	
	/****************************************************************************** R ******************************************************************************/

	virtual void R() override;
	void R_BusterShot();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"));
	class UProjectileMovementComponent* ProjectileMovementComponent;

	/*************************************************************************** Passive ***************************************************************************/

	/************************************************************************ Basic Attack ************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Impulse = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Speed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;

	TObjectPtr<AOPTristanaCannonBall> BasicAttack_CannonBallStorage; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaCannonBall> BasicAttack_CannonBallClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack", meta = (AllowPrivateAccess = "true"))
	const TObjectPtr<USkeletalMeshSocket> ProjectileSpawnSocket;

	FTimerHandle BasicAttack_CannonBallSpawnTimerHandle; 
	
	float BasicAttack_DefaultAttackSpeed;
	
	/****************************************************************************** Q ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	float Q_RapidFireValue = 0.5f;
	
	FTimerHandle Q_TimerHandle; 
	bool bIsQActive = false; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	float Q_Duration = 10.f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> Q_NiagaraComponent;

	/****************************************************************************** W ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_MaxJumpRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_JumpAngle = 0.f; // ??? 4 ???

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_JumpSpeed = 0.f; // ??? 4 ???

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_JumpStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_LandingImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_LandingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_SlowDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_LandingImpulseAngle;

	FTimerHandle W_JumpTimerHandle; 
	FVector W_FinalLocation;

	bool bIsWJumping;
	
	/****************************************************************************** E ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	AOPDiavolo* E_Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float E_Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_TimeToExplode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_MaxThrowRange;

	TObjectPtr<AOPTristanaExplosiveCharge> E_ExplosiveChargeStorage; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill3", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaExplosiveCharge> E_ExplosiveChargeClass; 
	
	AOPTristanaExplosiveCharge* E_CurrentExplosiveCharge;

	FTimerHandle E_SpawnTimerHandle;
	
	/****************************************************************************** R ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Impulse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Rebound = 5000.f;
	
	TObjectPtr<AOPTristanaBusterShot> R_BusterShotStorage; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPTristanaBusterShot> R_BusterShotClass; 

	FTimerHandle R_SpawnTimerHandle; 
};
