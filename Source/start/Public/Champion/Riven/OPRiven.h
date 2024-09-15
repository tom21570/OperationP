// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPRiven.generated.h"

class UNiagaraComponent;
class AOPRivenSlash;
class UNiagaraSystem;
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
	void Q_Trace() const;
	void Q_Trace_Third() const;
	void Q_OnLanding();
	
	/****************************************************************************** W ******************************************************************************/

	virtual void W() override;
	void W_Trace() const;
	
	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;

	UFUNCTION()
	void E_Reflect(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/****************************************************************************** R ******************************************************************************/
	
	virtual void R() override;
	void R_Slash();

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
	float Q_Speed_XY_ThirdShot = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_Range = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float Q_Width = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> Q_NiagaraSystem_Slash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> Q_NiagaraSystem_Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> Q_NiagaraSystem_OnLanding;
	
    int32 Q_Step = 1;
	bool bQ_IsInAir = false;

    FTimerHandle Q_Trace_TimerHandle;
    FTimerHandle Q_StackMaintain_TimerHandle;
    
    /****************************************************************************** W ******************************************************************************/
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_Angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_Width = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> W_NiagaraSystem;

	FTimerHandle W_Cast_TimerHandle;
    
    /****************************************************************************** E ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_ReflectStrength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Speed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_MaintainTime = 0.f;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UStaticMeshComponent> E_ShieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> E_NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_ReflectAngle = 0.f;

    FTimerHandle E_ShieldMaintain_TimerHandle;

	FVector E_FinalLocation; 
    
    /****************************************************************************** R ******************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> R_SwordMesh;

	TArray<AOPRivenSlash*> R_SlashStorages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPRivenSlash> R_SlashClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> R_SlashSpawnPoints;
	
    FTimerHandle R_Reinforce_TimerHandle;
	FTimerHandle R_ReinforceEnd_TimerHandle;
    FTimerHandle R_Slash_TimerHandle;

	bool bR_IsActivated = false;
	bool bR_CanSlash = false;
};
