// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "Components/TimelineComponent.h"
#include "OPGalio.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class AOPGalioWindblast;
/**
 * 
 */
UCLASS()
class START_API AOPGalio : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPGalio();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	/*************************************************************************** Passive ***************************************************************************/

	/************************************************************************ Basic Attack ************************************************************************/

	virtual void BasicAttack() override;
	void BasicAttack_Trace(const int32& Section) const;
	void BasicAttack_Trace_Passive() const;
	void BasicAttack_Trace_Up() const;
	void BasicAttack_Trace_Right() const;
	void BasicAttack_Trace_Left() const;
	void BasicAttack_ResetComboCount() { BasicAttack_ComboCount = 0; }
	
	/****************************************************************************** Q ******************************************************************************/

	virtual void Q() override;
	void Q_SpawnWindblast();
	
	/****************************************************************************** W ******************************************************************************/

	virtual void W() override;
	void W_Charge();
	void W_Recast();
	UFUNCTION()
	void W_TimelineProgress(float Value);
	
	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;
	void E_Trace() const;
	
	/****************************************************************************** R ******************************************************************************/

	virtual void R() override;
	void R_OnLanding();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent* ProjectileMovementComponent;

	/*************************************************************************** Passive ***************************************************************************/
    
    /************************************************************************ Basic Attack ************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float BasicAttack_Strength = 6000000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Strength_Passive = 13300000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Width = 3000.f;

	int32 BasicAttack_ComboCount = 0;
    FTimerHandle BasicAttack_Cast_TimerHandle;
    FTimerHandle BasicAttack_ComboCount_TimerHandle;
    
    /****************************************************************************** Q ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPGalioWindblast> Q_Windblast_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Q_Windblast_SpawnPoint_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Q_Windblast_SpawnPoint_2;
	
	TObjectPtr<AOPGalioWindblast> Q_Windblast_Storage_1;
	TObjectPtr<AOPGalioWindblast> Q_Windblast_Storage_2;

	FVector Q_FinalLocation;
	FTimerHandle Q_Cast_TimerHandle;
    
    /****************************************************************************** W ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float W_PullStrength = 3500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float W_StartRange = 3500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	float W_EndRange = 7000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> W_NiagaraSystem_Charging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> W_NiagaraSystemComponent_Charging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> W_NiagaraSystem_Recast;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* W_ChargeCurve;

	FTimeline W_Timeline;
	float W_FinalRange;
	bool bW_IsCharging = false;
	FTimerHandle W_CastTimer;
    
    /****************************************************************************** E ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Strength = 6300000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
    float E_Radius = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed = 153800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed_Backward = 15380.f;

	FTimerHandle E_DashStart_TimerHandle;
	FTimerHandle E_DashEnd_TimerHandle;
	bool bE_IsDashing;
    
    /****************************************************************************** R ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Strength = 2050000000000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed_Z_Departure = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed_XY_Landing = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed_Z_Landing = 117000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Radius = 13000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Gravity = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> R_NiagaraSystem_Departure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> R_NiagaraSystem_InAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> R_NiagaraSystem_SonicBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> R_NiagaraSystem_Land;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	FVector R_NiagaraSystem_Land_Size = FVector(1.f, 1.f, 1.f);

	FVector R_FinalLocation;
	bool bR_IsInAir = false;
	FTimerHandle R_Departure_TimerHandle;
	FTimerHandle R_SpawnNiagara_InAir_TimerHandle;
	FTimerHandle R_StartLanding_TimerHandle;
	FTimerHandle R_SpawnNiagara_TimerHandle;

public:
	FORCEINLINE bool GetbW_Charging() const { return bW_IsCharging; }
	FORCEINLINE bool GetbR_IsInAir() const { return bR_IsInAir; }
};
