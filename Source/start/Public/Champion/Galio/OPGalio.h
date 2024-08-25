// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "Components/TimelineComponent.h"
#include "OPGalio.generated.h"

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
	void BasicAttack_Trace();
	void BasicAttacK_Trace_Passive();
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
	void E_Trace();
	
	/****************************************************************************** R ******************************************************************************/

	virtual void R() override;
	void R_Trace();
	void R_OnLanding();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent* ProjectileMovementComponent;

	/*************************************************************************** Passive ***************************************************************************/
    
    /************************************************************************ Basic Attack ************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float BasicAttack_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Strength_Passive = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Width = 40.f;

	int32 BasicAttack_ComboCount = 0;
    FTimerHandle BasicAttack_Cast_TimerHandle;
    FTimerHandle BasicAttack_ComboCount_TimerHandle;
    
    /****************************************************************************** Q ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_WindStrength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_TornadoStrength = 0.f;

	TObjectPtr<AOPGalioWindblast> Q_Windblast_Storage_1;
	TObjectPtr<AOPGalioWindblast> Q_Windblast_Storage_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPGalioWindblast> Q_Windblast_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Q_Windblast_SpawnPoint_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | GameplayMethods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Q_Windblast_SpawnPoint_2;

	FVector Q_FinalLocation;
	FTimerHandle Q_Cast_TimerHandle;
    
    /****************************************************************************** W ******************************************************************************/

	FTimeline W_Timeline;
	float W_StartDistance;
	float W_EndDistance;
	float W_FinalDistance;
	bool bW_Charging;
	FTimerHandle W_CastTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* W_ChargeCurve;
    
    /****************************************************************************** E ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
    float E_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed_Backward = 0.f;

	FTimerHandle E_DashStart_TimerHandle;
	bool bE_IsDashing;
    
    /****************************************************************************** R ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed_Z_Departure = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed_XY_Landing = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Speed_Z_Landing = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Radius = 0.f;

	FVector R_FinalLocation;
	bool bR_IsInAir = false;
	FTimerHandle R_Departure_TimerHandle;
	FTimerHandle R_StartLanding_TimerHandle;
};
