// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OPTaric.generated.h"

class UBoxComponent;
class UOPTaricShield;
class UNiagaraSystem;
using namespace EDrawDebugTrace;

/**
 * 
 */
UCLASS()
class START_API AOPTaric : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPTaric();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/*************************************************************************** Passive ***************************************************************************/

	/************************************************************************ Basic Attack ************************************************************************/

	virtual void BasicAttack() override;
	void BasicAttack_Trace();
	void BasicAttack_Trace_Passive();
	void BasicAttack_ResetComboCount() { BasicAttack_ComboCount = 0; }
	void BasicAttack_ResetComboCount_Passive() { BasicAttack_ComboCount_Passive = 0; }
	
	/****************************************************************************** Q ******************************************************************************/

	virtual void Q() override;
	
	/****************************************************************************** W ******************************************************************************/

	virtual void W() override;
	UFUNCTION()
	void OnFragileCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;
	void E_Trace(const FVector& StartLocation, const FVector& Direction);
	
	/****************************************************************************** R ******************************************************************************/

	virtual void R() override;
	void R_Trace(const FVector& StartLocation);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent* ProjectileMovementComponent;

	/*************************************************************************** Passive ***************************************************************************/

	bool bPassive_IsOn = false;
    
    /************************************************************************ Basic Attack ************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float BasicAttack_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Passive_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Range = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float BasicAttack_Width = 0.f;

    int32 BasicAttack_ComboCount = 0;
    int32 BasicAttack_ComboCount_Passive = 0;
    FTimerHandle BasicAttack_ComboCount_TimerHandle;
    FTimerHandle BasicAttack_ComboCount_Passive_TimerHandle;
    FTimerHandle BasicAttack_Trace_TimerHandle;
	FTimerHandle BasicAttack_Trace_Passive_TimerHandle;
    
    /****************************************************************************** Q ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Mathods", meta = (AllowPrivateAccess = "true"))
    float Q_HealAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Mathods", meta = (AllowPrivateAccess = "true"))
	float Q_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> Q_NiagaraSystem;

    int32 Q_Stack = 0;
    
    /****************************************************************************** W ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_MaintainTime = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_Strength = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_ReflectAngle = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNiagaraSystem> W_Shield_NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> W_ShieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AOPChampion> W_TargetAlly;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOPTaricShield> W_Shield_GeometryCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> FragilePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W |Physical Tests", meta = (AllowPrivateAccess = "true"))
	TArray<float> CustomDamageThreshold;

    FTimerHandle W_Maintain_TimerHandle;
	bool bW_IsTethered = false;
    
    /****************************************************************************** E ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Range = 575.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Width = 70.f;

    FVector E_AllyVectorFor_Trace;
    FTimerHandle E_Trace_TimerHandle_Taric;
    FTimerHandle E_Trace_TimerHandle_Ally;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	bool bE_DrawDebugTrace = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> E_NiagaraSystem;
    
    /****************************************************************************** R ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Angle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Radius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	bool bR_DrawDebugTrace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> R_BallClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> R_Burst_NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> R_Casting_NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> R_Shield_NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterial> R_Taric_Material;

	TObjectPtr<UGeometryCollectionComponent> GC;

    FTimerHandle R_Trace_TimerHandle_Taric;
    FTimerHandle R_Trace_TimerHandle_Ally;

	FTimerHandle R_NiagaraBall_Spawn_TimerHandle;
};
