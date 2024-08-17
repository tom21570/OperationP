// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPLeeSin.generated.h"

class USphereComponent;
class AOPLeeSinSonicWave;
class AOPLeeSinDragonsRage;

UCLASS()
class START_API AOPLeeSin : public AOPChampion
{
    GENERATED_BODY()

public:
    AOPLeeSin();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
protected:
    /*************************************************************************** Passive ***************************************************************************/
    
    virtual void Passive() override;

    /************************************************************************ Basic Attack ************************************************************************/
    
    virtual void BasicAttack() override;
    bool BasicAttackTrace();
    void BasicAttack_ResetComboCount();

    /****************************************************************************** Q ******************************************************************************/

    virtual void Q() override;
    void Q_SonicWave();
    
    /****************************************************************************** W ******************************************************************************/

    virtual void W() override;

    UFUNCTION()
    void W_OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    /****************************************************************************** E ******************************************************************************/

    virtual void E() override;
    void E_GroundSlam();
    void E_Cripple();
    
    /****************************************************************************** R ******************************************************************************/
    
    virtual void R() override;
    bool R_Trace();

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent* ProjectileMovementComponent;
    
    /************************************************************************ Basic Attack ************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float BasicAttack_Strength = 0.f;

    int32 BasicAttackComboCount = 0;
    FTimerHandle BasicAttack_ComboCount_TimerHandle;
    FTimerHandle BasicAttack_Trace_TimerHandle;
    
    /****************************************************************************** Q ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_SonicWaveStrength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_SonicWaveSpeed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_ResonateSpeed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_ResonateStrength = 0.f;

    TObjectPtr<AOPLeeSinSonicWave> Q_SonicWaveStorage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Q", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AOPLeeSinSonicWave> Q_SonicWaveClass;

    int32 Q_Stack = 0;
    
    bool bQ_CanResonate = false;
    bool bQ_IsResonating = false;
    TObjectPtr<AOPChampion> Q_ResonateTarget;

    FTimerHandle Q_Cast_TimerHandle;
    FTimerHandle Q_ResonatingStrikeDiavoloMotion_TimerHandle;
    FTimerHandle Q_SonicWaveSpawn_TimerHandle;
    
    /****************************************************************************** W ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_MaintainTime = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_Strength = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_ReflectAngle = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UStaticMeshComponent> W_SafeGuardMesh;

    FTimerHandle W_Maintain_TimerHandle;
    
    /****************************************************************************** E ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Radius = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "E", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* E_MarkerMesh;

    int32 E_Stack = 0;
    FTimerHandle E_Cast_TimerHandle;
    FTimerHandle E_Stack_TimerHandle;
    bool bE_SecondInput = false;
    FTimerHandle E_End_TimerHandle;
    
    /****************************************************************************** R ******************************************************************************/

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "R | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Angle = 0.f;

    FTimerHandle R_Trace_TimerHandle;

public:    
    void CreateMarkerOnTarget(AOPDiavolo* Target);
    void RemoveMarkerOnTarget(AOPDiavolo* Target);

    FORCEINLINE bool GetbCanResonate() const { return bQ_CanResonate; }
    FORCEINLINE void SetbCanResonate_True() { bQ_CanResonate = true; }
    FORCEINLINE void SetbCanResonate_False() { bQ_CanResonate = false; }

    FORCEINLINE bool GetbIsResonating() const { return bQ_IsResonating; }
    FORCEINLINE void SetbIsResonating_True() { bQ_IsResonating = true; }
    FORCEINLINE void SetbIsResonating_False() { bQ_IsResonating = false; }

    FORCEINLINE void SetResonateTarget(AOPChampion* Target) { Q_ResonateTarget = Target; }
};
