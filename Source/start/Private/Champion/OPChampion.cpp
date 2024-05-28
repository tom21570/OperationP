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

	static ConstructorHelpers::FObjectFinder<UInputAction> Skill1Input(TEXT("/Game/Input/IA_Skill_1.IA_Skill_1"));
	if(MoveInput.Succeeded())
	{
		SkillAction1 = Skill1Input.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Skill2Input(TEXT("/Game/Input/IA_Skill_2.IA_Skill_2"));
	if(MoveInput.Succeeded())
	{
		SkillAction2 = Skill2Input.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Skill3Input(TEXT("/Game/Input/IA_Skill_3.IA_Skill_3"));
	if(MoveInput.Succeeded())
	{
		SkillAction3 = Skill3Input.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Skill4Input(TEXT("/Game/Input/IA_Skill_4.IA_Skill_4"));
	if(MoveInput.Succeeded())
	{
		SkillAction4 = Skill4Input.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> UltInput(TEXT("/Game/Input/IA_Ult.IA_Ult"));
	if(MoveInput.Succeeded())
	{
		UltAction = UltInput.Object;
	}
	
	ChampionAnimInstance = Cast<UOPAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called when the game starts or when spawned
void AOPChampion::BeginPlay()
{
	Super::BeginPlay();

	OPPlayerController = Cast<AOPPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	check(OPPlayerController);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OPPlayerController->GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(ChampionMappingContext, 0);

	ChampionAnimInstance = Cast<UOPAnimInstance>(GetMesh()->GetAnimInstance());
}

void AOPChampion::Passive()
{
	UE_LOG(LogTemp, Warning, TEXT("Passive"));
}

void AOPChampion::MeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MeleeAttack"));
}

void AOPChampion::Skill_1()
{
	UE_LOG(LogTemp, Warning, TEXT("Skill 1"));
}

void AOPChampion::Skill_2()
{
	UE_LOG(LogTemp, Warning, TEXT("Skill 2"));
}

void AOPChampion::Skill_3()
{
	UE_LOG(LogTemp, Warning, TEXT("Skill 3"));
}

void AOPChampion::Skill_4()
{
	UE_LOG(LogTemp, Warning, TEXT("Skill 4"));
}

void AOPChampion::Ult()
{
	UE_LOG(LogTemp, Warning, TEXT("Ult"));
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

void AOPChampion::PlayDiavoloRandomDeadMontage()
{
	if (TestDiavolo == nullptr) return;
	int32 Section = FMath::RandRange(0, 4);
	FTimerHandle DeadTimer;
	TestDiavolo->GetChampionAnimInstance()->Montage_Play(TestDiavolo->GetDeadAnimMontage());
	switch (Section)
	{				
	// case 0:
	// 	TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_1"));
	// 	GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
	// 	{
	// 		TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_1"));
	// 	}), 3.9f, false);
	// 	break;
				
	case 0:
		TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_2"));
		GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
		{
			TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_2"));
		}), 4.45f, false);
		break;
				
	case 1:
		TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_3"));
		GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
		{
			TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_3"));
		}), 3.5f, false);
		break;
				
	case 2:
		TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_4"));
		GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
		{
			TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_4"));
		}), 3.4f, false);
		break;

	case 3:
		TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dying_Backwards"));
		GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateLambda([&]
		{
			TestDiavolo->GetChampionAnimInstance()->Montage_JumpToSection(FName("Dead_Backwards"));
		}), 3.85f, false);
		break;
			
	default:
		break;
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

	EnhancedInputComponent->BindAction(MeleeAttackAction, ETriggerEvent::Started, this, &AOPChampion::MeleeAttack);
	EnhancedInputComponent->BindAction(SkillAction1, ETriggerEvent::Started, this, &AOPChampion::Skill_1);
	EnhancedInputComponent->BindAction(SkillAction2, ETriggerEvent::Started, this, &AOPChampion::Skill_2);
	EnhancedInputComponent->BindAction(SkillAction3, ETriggerEvent::Started, this, &AOPChampion::Skill_3);
	EnhancedInputComponent->BindAction(SkillAction4, ETriggerEvent::Started, this, &AOPChampion::Skill_4);
	EnhancedInputComponent->BindAction(UltAction, ETriggerEvent::Started, this, &AOPChampion::Ult);
}

