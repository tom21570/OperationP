// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPLeeSin.generated.h"

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
    virtual void MeleeAttack() override;
    bool MeleeAttackTrace();
    virtual void Skill_1() override;
    bool Skill_1_Trace();
    void Skill_1_SonicWave();

    virtual void Skill_2() override;
    virtual void Skill_3() override;
    virtual void Ult() override;
    bool UltTrace();

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Movement component", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float MeleeAttack_Impulse = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Skill_1_Impulse = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Skill_1_Velocity = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Skill_3_Impulse = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Skill_3_radious = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Skill_3_slowAmount = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Skill_3_slowDuration = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Ult_Impulse = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float Ult_Angle = 0.f;

    int32 MeleeAttackComboCount = 0;
    FTimerHandle MeleeAttackComboCountTimer;

    FTimerHandle Skill_1_CastTimer;

    int32 Skill_3_Stack = 0;
    FTimerHandle Skill_3_CastTimer;
    FTimerHandle Skill_3_StackTimer;

    TObjectPtr<AOPLeeSinSonicWave> SonicWave;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill 1", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AOPLeeSinSonicWave> SonicWaveClass;

    FTimerHandle SonicWaveSpawnTimer;
    FTimerHandle DragonsRageSpawnTimer;

    TObjectPtr<AOPLeeSinDragonsRage> DragonsRage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ult", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AOPLeeSinDragonsRage> DragonsRageClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* MarkerMesh;

    void ApplyShieldToAlly(AOPChampion* TargetChampion);
    void AddShield(float ShieldAmount);
    void Skill_3_GroundSlam();
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


};