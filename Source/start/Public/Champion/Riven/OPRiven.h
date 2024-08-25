// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPRiven.generated.h"

/**
 * 
 */
UCLASS()
class START_API AOPRiven : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPRiven();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
protected:
	/*************************************************************************** Passive ***************************************************************************/

	/************************************************************************ Basic Attack ************************************************************************/

	virtual void BasicAttack() override;
	void BasicAttack_Trace();
	
	/****************************************************************************** Q ******************************************************************************/

	virtual void Q() override;
	void Q_Trace();
	void Q_Trace_Third();
	
	/****************************************************************************** W ******************************************************************************/

	virtual void W() override;
	void W_Trace();
	
	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;
	
	/****************************************************************************** R ******************************************************************************/
	
	virtual void R() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent* ProjectileMovementComponent;
    
    /************************************************************************ Basic Attack ************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float BasicAttack_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_R_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Width = 0.f;

    int32 BasicAttackComboCount = 0;
    FTimerHandle BasicAttack_ComboCount_TimerHandle;
    FTimerHandle BasicAttack_Trace_TimerHandle;
    
    /****************************************************************************** Q ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_AirborneAngle = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_Speed_XY = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_Speed_Z = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_Range = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_Width = 100.f;

    int32 Q_Step = 1;

    FTimerHandle Q_Trace_TimerHandle;
    FTimerHandle Q_StackMaintain_TimerHandle;
    
    /****************************************************************************** W ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_Angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_Width = 150.f;
    
    /****************************************************************************** E ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* E_ShieldMesh;

    FTimerHandle E_ShieldMaintain_TimerHandle;

	FVector E_FinalLocation; 
    
    /****************************************************************************** R ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> R_SwordMesh;
	
    FTimerHandle R_Cast_TimerHandle;

	bool bR_IsActivated = false;
	bool bR_CanSlash = false;
};
