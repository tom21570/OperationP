// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OPOriginalCharacter.generated.h"

class UInputAction;

UCLASS()
class START_API AOPOriginalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AOPOriginalCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/********************************************************************** Player Controller **********************************************************************/
	
	FHitResult MouseCursorHit;
	TObjectPtr<class AOPPlayerController> OCPlayerController;

	/************************************************************************ Player Sight ************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> OCMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> OCSkillAction;	

	virtual void Skill();

	TObjectPtr<class UOPAnimInstance> OCAnimInstance;

public:
	void TurnCharacterToLocation(const FVector& TurnPoint);
	void TurnCharacterToLocation_3D(const FVector& TurnPoint);
	void TurnCharacterToCursor(const FHitResult& HitResult);
	void TurnCharacterToCursor_3D(const FHitResult& HitResult);
};
