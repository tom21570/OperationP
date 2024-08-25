// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPKennenShuriken.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EShurikenType : uint8
{
	Shuriken_BasicAttack	UMETA(DisplayName = "Shuriken BasicAttack"),
	Shuriken_Q				UMETA(DisplayName = "Shuriken Q"),
	Shuriken_W				UMETA(DisplayName = "Shuriken W")
};

/**
 * 
 */
UCLASS()
class START_API AOPKennenShuriken : public AOPProjectile
{
	GENERATED_BODY()

public:
	AOPKennenShuriken();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shuriken Type", meta = (AllowPrivateAccess = "true"))
	EShurikenType ShurikenType;

public:
	FORCEINLINE EShurikenType GetShurikenType() const { return ShurikenType; }
	FORCEINLINE void SetShurikenType(EShurikenType Value) { ShurikenType = Value; }
};
