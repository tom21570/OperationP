// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomGravityActor.generated.h"

UCLASS()
class START_API ACustomGravityActor : public AActor
{
    GENERATED_BODY()

    public:
    ACustomGravityActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

};