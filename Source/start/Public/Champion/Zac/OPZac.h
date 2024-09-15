// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPZac.generated.h"

class AOPZacChunk;
class USphereComponent;
class UNiagaraSystem;
class UPhysicsHandleComponent;
/**
 * 
 */
UCLASS()
class START_API AOPZac : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPZac();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	/*************************************************************************** Passive ***************************************************************************/

	virtual void Passive() override;
	UFUNCTION(BlueprintCallable)
	void ChangeZacSize();

	/************************************************************************ Basic Attack ************************************************************************/

	virtual void BasicAttack() override;
	void BasicAttack_Trace();
	
	/****************************************************************************** Q ******************************************************************************/

	virtual void Q() override;
	void Q_Trace_Catch_First();
	void Q_Trace_Catch_Second();
	void Q_Slam();
	
	/****************************************************************************** W ******************************************************************************/

	virtual void W() override;
	void W_Trace();
	
	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;
	void E_OnLanding();
	
	/****************************************************************************** R ******************************************************************************/

	virtual void R() override;
	void R_OnLanding();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
	bool bZac_IsBig = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent* ProjectileMovementComponent;

	/*************************************************************************** Passive ***************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Passive_SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPZacChunk> Passive_ChunkClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Zac_IncreasedSizeIndex = 1.45f;

	TObjectPtr<AOPZacChunk> Passive_ChunkStorage;
	FVector Zac_OriginalSizeIndex;
    
    /************************************************************************ Basic Attack ************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float BasicAttack_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Width = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Strength_Big = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range_Big = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Width_Big = 40.f;
	
    FTimerHandle BasicAttack_Trace_TimerHandle;
    
    /****************************************************************************** Q ******************************************************************************/

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle_RightHand;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle_LeftHand;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_ArmStretchStrength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_SlamStrength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_SlamAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Q_CaughtMaintainTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Q_Width = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_ArmStretchStrength_Big = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_SlamStrength_Big = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_SlamAngle_Big = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Q_CaughtMaintainTime_Big = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float Q_Width_Big = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Q_GrabPoint_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Q_GrabPoint_LeftHand;
	
    bool bQ_CaughtTarget_LeftHand = false;
    bool bQ_CaughtTarget_RightHand = false;
	bool bQ_IsStretching_LeftHand = false;
	bool bQ_IsStretching_RightHand = false;
	bool bQ_IsReadyToSlam = false;

	int32 Q_TickVariable = 0.f;
	FVector Q_FinalLocation;
	
    TObjectPtr<AOPChampion> Q_CaughtTarget;
    TObjectPtr<AOPChampion> Q_SlamTarget;

    FTimerHandle Q_StretchCast_LeftHand_TimerHandle;
    FTimerHandle Q_StretchCast_RightHand_TimerHandle;
	FTimerHandle Q_StretchEnd_RightHand_TimerHandle;
	FTimerHandle Q_StretchEnd_LeftHand_TimerHandle;
	FTimerHandle Q_SlamCast_TimerHandle;
    
    /****************************************************************************** W ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float W_Strength = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float W_Strength_Big = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float W_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float W_Radius_Big = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> Zac_Splash_NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> Zac_Splash_NiagaraSystem_Big;
    
    /****************************************************************************** E ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
    float E_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed_XY = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed_Z = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Strength_Big = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float E_Radius_Big = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed_XY_Big = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed_Z_Big = 0.f;

    bool bE_CanJump = false;
	bool bE_IsInAir = false;
    
    /****************************************************************************** R ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float R_Radius = 0.f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed_Z = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Strength_Big = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float R_Radius_Big = 0.f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed_Z_Big = 0.f;

	bool bR_IsInAir = false;

	FTimerHandle R_StartIsInAirLoop_TimeHandle;
    FTimerHandle R_TraceStart_TimerHandle;
    FTimerHandle R_TraceLoop_TimerHandle;
    FTimerHandle R_TraceStop_TimerHandle;

public:
	FORCEINLINE bool GetbEIsInAir() const  { return bE_IsInAir; }
	FORCEINLINE bool GetbRIsInAir() const  { return bR_IsInAir; }
};
