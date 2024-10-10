// Fill out your copyright notice in the Description page of Project Settings.


#include "OriginalCharacter/OPOriginalCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/OPAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/OPPlayerController.h"

// Sets default values
AOPOriginalCharacter::AOPOriginalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OCPlayerController = Cast<AOPPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 0.f); // 90, 30

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	OCAnimInstance = Cast<UOPAnimInstance>(GetMesh()->GetAnimInstance());
}

void AOPOriginalCharacter::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(UGameplayStatics::GetPlayerController(this, 0)->GetLocalPlayer());
	if (Subsystem == nullptr) return;
	Subsystem->AddMappingContext(OCMappingContext, 0);

	OCAnimInstance = Cast<UOPAnimInstance>(GetMesh()->GetAnimInstance());
}

void AOPOriginalCharacter::Skill()
{
}

void AOPOriginalCharacter::TurnCharacterToLocation(const FVector& TurnPoint)
{
	FVector CursorLocation{TurnPoint.X, TurnPoint.Y, this->GetActorLocation().Z};
	FVector StartPoint{this->GetActorLocation()};

	FRotator Rotation = FVector(CursorLocation - StartPoint).Rotation();
	SetActorRotation(Rotation);
}

void AOPOriginalCharacter::TurnCharacterToLocation_3D(const FVector& TurnPoint)
{
	FVector CursorLocation{TurnPoint.X, TurnPoint.Y, TurnPoint.Z};
	FVector StartPoint{this->GetActorLocation()};

	FRotator Rotation = FVector(CursorLocation - StartPoint).Rotation();
	SetActorRotation(Rotation);
}

void AOPOriginalCharacter::TurnCharacterToCursor(const FHitResult& HitResult)
{
	TurnCharacterToLocation(HitResult.Location);
}

void AOPOriginalCharacter::TurnCharacterToCursor_3D(const FHitResult& HitResult)
{
	TurnCharacterToLocation_3D(HitResult.Location);
}

void AOPOriginalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOPOriginalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);
	
	EnhancedInputComponent->BindAction(OCSkillAction, ETriggerEvent::Started, this, &AOPOriginalCharacter::Skill);
}
