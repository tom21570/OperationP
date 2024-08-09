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

protected:
    virtual void BeginPlay() override;

    virtual void Passive() override;
    virtual void BasicAttack() override;
    bool BasicAttackTrace();
    virtual void Q() override;
    void Skill_1_SonicWave();

    virtual void W() override;
    virtual void E() override;
    virtual void R() override;
    bool UltTrace();

private:    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Attack | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float BasicAttack_Impulse = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_Speed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_ResonateSpeed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1 | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Q_ResonateStrength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "W | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float W_MaintainTime = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_Radius = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_slowAmount = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 3 | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float E_slowDuration = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Strength = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate | Physical Tests", meta = (AllowPrivateAccess = "true"))
    float R_Angle = 0.f;

    int32 BasicAttackComboCount = 0;
    FTimerHandle BasicAttackComboCountTimerHandle;
    FTimerHandle BasicAttackTraceTimerHandle;
    
    FTimerHandle Skill_1_CastTimer;
    FTimerHandle ResonatingStrike_DiavoloMotionTimer;

    FTimerHandle W_MaintainTimerHandle;
    int32 Skill_3_Stack = 0;
    FTimerHandle Skill_3_CastTimer;
    FTimerHandle Skill_3_StackTimer;

    TObjectPtr<AOPLeeSinSonicWave> SonicWave;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AOPLeeSinSonicWave> SonicWaveClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UStaticMeshComponent> SafeGuardMesh;

    bool bCanResonate = false;
    bool bIsResonating = false;
    TObjectPtr<AOPChampion> ResonateTarget;

    bool bE_SecondInput = false;

    FTimerHandle SonicWaveSpawnTimer;
    FTimerHandle E_EndTimer;
    FTimerHandle DragonsRageSpawnTimer;

    TObjectPtr<AOPLeeSinDragonsRage> DragonsRage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AOPLeeSinDragonsRage> DragonsRageClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* MarkerMesh;

    void ApplyShieldToAlly(AOPChampion* TargetChampion);
    void AddShield(float ShieldAmount);
    void Skill_3_GroundSlam();
    void Skill_3_Cripple();
    void Skill_3_ApplySlowEffect();
    void OnDashCompleted();
    void ResetMeleeAttackComboCount();
    void DashToTarget(AOPChampion* TargetChampion, float DashSpeed, float DashDistance);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
    float ShieldAmountValue;

    FTimerHandle DashCompleteTimer;

public:
    int32 Skill_1_Stack = 0;
    FTimerHandle Skill_1_StackTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
    float Skill_1_StackTime = 0.25f;

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void CreateMarkerOnTarget(AOPDiavolo* Target);
    void RemoveMarkerOnTarget(AOPDiavolo* Target);

    FORCEINLINE bool GetbCanResonate() const { return bCanResonate; }
    FORCEINLINE void SetbCanResonate_True() { bCanResonate = true; }
    FORCEINLINE void SetbCanResonate_False() { bCanResonate = false; }

    FORCEINLINE bool GetbIsResonating() const { return bIsResonating; }
    FORCEINLINE void SetbIsResonating_True() { bIsResonating = true; }
    FORCEINLINE void SetbIsResonating_False() { bIsResonating = false; }

    FORCEINLINE void SetResonateTarget(AOPChampion* Target) { ResonateTarget = Target; }
};
