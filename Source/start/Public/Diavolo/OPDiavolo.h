// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Champion/OPChampion.h"
#include "OPDiavolo.generated.h"

UCLASS()
class START_API AOPDiavolo : public AOPChampion
{
    GENERATED_BODY()

public:
    AOPDiavolo();    
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;
    void ApplySlowEffect(float SlowAmount, float Duration);
    void ApplySlowAttackEffect(float SlowAmount, float Duration);
    void ResetSpeed();

protected:
    void PlayDeadMontage1();
    void PlayDeadMontage2();
    void PlayDeadMontage3();
    void PlayDeadMontage4();
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* MarkerMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    bool bCanBeTestedMultipleTimes = false;

    /***********당구대 반사 관련**********/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    float ReflectedVelocityRatio = 0.8f;
    /***********당구대 반사 관련**********/

    FTimerHandle SlowEffectTimerHandle;
    float OriginalSpeed;

    FTimerHandle DeadTimer1;
    FTimerHandle DeadTimer2;
    FTimerHandle DeadTimer3;
    FTimerHandle DeadTimer4;

    /************* 리신 상대 애님 몽타주 **************/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeSin", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Diavolo_DamagedByLeeSinMeleeAttack_AnimMontage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeSin", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Diavolo_DamagedByLeeSinSonicWave_AnimMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeSin", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Diavolo_DamagedByLeeSinResonatingStrike_AnimMontage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeSin", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Diavolo_DamagedByLeeSinSkill_3_AnimMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeSin", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> Diavolo_DamagedByLeeSinDragonsRage_AnimMontage;

public:
    FORCEINLINE bool GetbCanBeTestedMultipleTimes() const { return bCanBeTestedMultipleTimes; }

    TObjectPtr<USkeletalMeshComponent> GetDiavoloMesh() const;

    void PlayDiavoloRandomDeadMontage();

    void PlayDiavoloRandomDamagedMontage();

    TObjectPtr<UAnimMontage> GetDiavolo_DamagedByLeeSinMeleeAttack_AnimMontage() { return Diavolo_DamagedByLeeSinMeleeAttack_AnimMontage; }
    TObjectPtr<UAnimMontage> GetDiavolo_DamagedByLeeSinSonicWave_AnimMontage() { return Diavolo_DamagedByLeeSinSonicWave_AnimMontage; }
    TObjectPtr<UAnimMontage> GetDiavolo_DamagedByLeeSinResonatingStrike_AnimMontage() { return Diavolo_DamagedByLeeSinResonatingStrike_AnimMontage; }
    TObjectPtr<UAnimMontage> GetDiavolo_DamagedByLeeSinSkill_3_AnimMontage() { return Diavolo_DamagedByLeeSinSkill_3_AnimMontage; }
    TObjectPtr<UAnimMontage> GetDiavolo_DamagedByLeeSinDragonsRage_AnimMontage() { return Diavolo_DamagedByLeeSinDragonsRage_AnimMontage; }

    FORCEINLINE UStaticMeshComponent* GetMarkerMesh() const { return MarkerMesh; }

};
