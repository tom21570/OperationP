// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPKennen.generated.h"

class UProjectileMovementComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class AOPKennenShuriken;
/**
 * 
 */
UCLASS()
class START_API AOPKennen : public AOPChampion
{
	GENERATED_BODY()

public:
	AOPKennen();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/*************************************************************************** Passive ***************************************************************************/

	/************************************************************************ Basic Attack ************************************************************************/

	virtual void BasicAttack() override;
	
	/****************************************************************************** Q ******************************************************************************/

	virtual void Q() override;
	
	/****************************************************************************** W ******************************************************************************/

	virtual void W() override;
	void W_Trace();
	void W_TraceForStormMark();
	
	/****************************************************************************** E ******************************************************************************/

	virtual void E() override;
	
	/****************************************************************************** R ******************************************************************************/

	virtual void R() override;
	void R_Trace();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ShurikenSpawnPoint;

	TObjectPtr<AOPKennenShuriken> KennenShurikenStorage;
	
	/*************************************************************************** Passive ***************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> Passive_NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passive | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> Passive_Stun_NiagaraSystem;
	
	FTimerHandle Passive_StormMarkMaintain_TimerHandle;
    
    /************************************************************************ Basic Attack ************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float BasicAttack_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPKennenShuriken> KennenShuriken_BasicAttack_Class;

	FTimerHandle BasicAttack_Cast_TimerHandle;
    
    /****************************************************************************** Q ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_Volt = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPKennenShuriken> KennenShuriken_Q_Class;

    FTimerHandle Q_Cast_TimerHandle;
    
    /****************************************************************************** W ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_Volt = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_Strength = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float W_Radius = 750.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPKennenShuriken> KennenShuriken_W_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Gameplay Methods", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> W_NiagaraSystem;

	int32 W_ReinforcedAttack_Stack = 0;
    
    /****************************************************************************** E ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Volt = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float E_Radius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> E_NiagaraComponent;

    FTimerHandle E_Maintain_TimerHandle;

	bool bE_IsRushing;
    
    /****************************************************************************** R ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Volt = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
	float R_Radius = 550.f;

    FTimerHandle R_TraceLoop_TimerHandle;
    FTimerHandle R_TraceStop_TimerHandle;

public:
	void Passive_StormMarkOthers(AOPChampion* Enemy);
	void Passive_StunOthers(AOPChampion* Enemy);
};
