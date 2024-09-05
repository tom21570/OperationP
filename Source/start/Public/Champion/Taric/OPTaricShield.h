// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "OPTaricShield.generated.h"

/**
 * 
 */
UCLASS()
class START_API UOPTaricShield : public UGeometryCollectionComponent
{
	GENERATED_BODY()

public:
	UOPTaricShield();
	virtual void BeginPlay() override;

protected:
	// UFUNCTION()
	// void OnFragileCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
private:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<USceneComponent> Prism_FragilePoint;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<USceneComponent> Frustum_FragilePoint;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<USceneComponent> Pyramid_FragilePoint;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<UBoxComponent> Prism_FragileCollision;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<UBoxComponent> Frustum_FragileCollision;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry", meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<UBoxComponent> Pyramid_FragileCollision;    
};
