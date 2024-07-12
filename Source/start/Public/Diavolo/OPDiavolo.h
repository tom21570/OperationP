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
    virtual void Tick(float DeltaTime) override;

    void ApplySlowEffect(float SlowAmount, float Duration);
    void ResetSpeed();

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical Tests", meta = (AllowPrivateAccess = "true"))
    bool bCanBeTestedMultipleTimes = false;

    FTimerHandle SlowEffectTimerHandle;
    float OriginalSpeed;

public:
    bool bTrueSightOn = false;

    FORCEINLINE bool GetbCanBeTestedMultipleTimes() const { return bCanBeTestedMultipleTimes; }

    USkeletalMeshComponent* GetDiavoloMesh() const;

};