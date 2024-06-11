// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/OPProjectile.h"
#include "OPYasuoWindWall.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class START_API AOPYasuoWindWall : public AOPProjectile
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	AOPYasuoWindWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* 나이아가라 시스템도 간단하기 때문에 BP에서 추가해주시는게 아마 더 편하실겁니다! */
	// UPROPERTY(EditDefaultsOnly, Category = "Effects")
	// TObjectPtr<UNiagaraSystem> ProjectileEffect;
	//
	// UPROPERTY(VisibleAnywhere, Category = "Effects")
	// TObjectPtr<UNiagaraComponent> ProjectileNiagaraComponent;

	FVector StartLocation;

	FTimerHandle StopTimerHandle;
	FTimerHandle DestroyTimerHandle;

	void StopProjectile();
	void DestroyProjectile();
	void ChangeSpeed();

	UFUNCTION()
	virtual void OnDamageCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitProjectile(FVector Direction, float Speed);

private:

	FTimerHandle DestroyTimer;
	FTimerHandle SpeedChangeTimer;
	
	// 모던 c++에선 클래스의 헤더파일에서 변수의 선언과 초기화를 동시에 해줘도 괜찮습니다! 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float InitialSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float FinalSpeed = 13.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float DistanceToStop = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float StopDuration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float WindWallForce = 0.f;
};