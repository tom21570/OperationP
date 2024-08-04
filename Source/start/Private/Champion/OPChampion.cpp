// Fill out your copyright notice in the Description page of Project Settings.


#include "Champion/OPChampion.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/OPPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/OPAnimInstance.h"
#include "Diavolo/OPDiavolo.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/DamageType.h" //damage
#include "Engine/EngineTypes.h" //damage
#include "Engine/World.h" //damage

// Sets default values
AOPChampion::AOPChampion()
{
	PrimaryActorTick.bCanEverTick = true;

	OPPlayerController = Cast<AOPPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

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
	GetCharacterMovement()->JumpZVelocity = 700.f;

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveInput(TEXT("/Game/Input/IA_Move.IA_Move"));
	if(MoveInput.Succeeded())
	{
		MoveAction = MoveInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookInput(TEXT("/Game/Input/IA_Look.IA_Look"));
	if(MoveInput.Succeeded())
	{
		LookAction = LookInput.Object;
	}

	// static ConstructorHelpers::FObjectFinder<UInputAction> JumpInput(TEXT("/Game/Input/IA_Jump.IA_Jump"));
	// if(MoveInput.Succeeded())
	// {
	// 	JumpAction = JumpInput.Object;
	// }

	static ConstructorHelpers::FObjectFinder<UInputAction> QInput(TEXT("/Game/Input/IA_Skill_1.IA_Skill_1"));
	if(MoveInput.Succeeded())
	{
		Q_Action = QInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> WInput(TEXT("/Game/Input/IA_Skill_2.IA_Skill_2"));
	if(MoveInput.Succeeded())
	{
		W_Action = WInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EInput(TEXT("/Game/Input/IA_Skill_3.IA_Skill_3"));
	if(MoveInput.Succeeded())
	{
		E_Action = EInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RInput(TEXT("/Game/Input/IA_Ult.IA_Ult"));
	if(MoveInput.Succeeded())
	{
		R_Action = RInput.Object;
	}
	
	ChampionAnimInstance = Cast<UOPAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called when the game starts or when spawned
void AOPChampion::BeginPlay()
{
	Super::BeginPlay();

	OPPlayerController = Cast<AOPPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (OPPlayerController == nullptr) return;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OPPlayerController->GetLocalPlayer());
	if (Subsystem == nullptr) return;
	Subsystem->AddMappingContext(ChampionMappingContext, 0);

	ChampionAnimInstance = Cast<UOPAnimInstance>(GetMesh()->GetAnimInstance());

	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void AOPChampion::PlayDeadAnimMontage() const
{
	ChampionAnimInstance->Montage_Play(DeadAnimMontage);
}

void AOPChampion::ResetChampionMovement() const
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 700.f;
}

void AOPChampion::StopChampionMovement() const
{
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->JumpZVelocity = 0.f;
}

void AOPChampion::Passive()
{
	UE_LOG(LogTemp, Warning, TEXT("Passive"));
}

void AOPChampion::BasicAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Basic Attack"));
}

void AOPChampion::Q()
{
	UE_LOG(LogTemp, Warning, TEXT("Q"));
}

void AOPChampion::W()
{
	UE_LOG(LogTemp, Warning, TEXT("W"));
}

void AOPChampion::E()
{
	UE_LOG(LogTemp, Warning, TEXT("E"));
}

void AOPChampion::R()
{
	UE_LOG(LogTemp, Warning, TEXT("R"));
}

void AOPChampion::TurnCharacterToLocation(FVector TurnPoint)
{
	FVector CursorLocation{TurnPoint.X, TurnPoint.Y, this->GetActorLocation().Z};
	FVector StartPoint{this->GetActorLocation()};

	FRotator Rotation = FVector(CursorLocation - StartPoint).Rotation();
	this->SetActorRotation(Rotation);
}

void AOPChampion::TurnCharacterToCursor(FHitResult HitResult)
{
	TurnCharacterToLocation(HitResult.Location);
}


void AOPChampion::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputAxisVector.Y);
	AddMovementInput(RightDirection, InputAxisVector.X);
}

void AOPChampion::Look(const FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	AOPPlayerController* OPPlayerControllerRef = Cast<AOPPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (OPPlayerControllerRef && OPPlayerControllerRef->IsInputKeyDown(EKeys::MiddleMouseButton))
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Called every frame
void AOPChampion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AOPChampion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);
	
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOPChampion::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOPChampion::Look);

	EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Started, this, &AOPChampion::BasicAttack);
	EnhancedInputComponent->BindAction(Q_Action, ETriggerEvent::Started, this, &AOPChampion::Q);
	EnhancedInputComponent->BindAction(W_Action, ETriggerEvent::Started, this, &AOPChampion::W);
	EnhancedInputComponent->BindAction(E_Action, ETriggerEvent::Started, this, &AOPChampion::E);
	EnhancedInputComponent->BindAction(R_Action, ETriggerEvent::Started, this, &AOPChampion::R);
}
