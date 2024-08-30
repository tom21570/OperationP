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

	static ConstructorHelpers::FObjectFinder<UInputAction> BasicAttackInput(TEXT("/Game/Input/IA_BasicAttack.IA_BasicAttack"));
	if(MoveInput.Succeeded())
	{
		BasicAttackAction = BasicAttackInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QInput(TEXT("/Game/Input/IA_Q.IA_Q"));
	if(MoveInput.Succeeded())
	{
		Q_Action = QInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> WInput(TEXT("/Game/Input/IA_W.IA_W"));
	if(MoveInput.Succeeded())
	{
		W_Action = WInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EInput(TEXT("/Game/Input/IA_E.IA_E"));
	if(MoveInput.Succeeded())
	{
		E_Action = EInput.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RInput(TEXT("/Game/Input/IA_R.IA_R"));
	if(MoveInput.Succeeded())
	{
		R_Action = RInput.Object;
	}
	
	// BP_GravityField_Sphere 블루프린트 클래스를 로드
	static ConstructorHelpers::FClassFinder<AActor> GravityFieldSphereBPClass(TEXT("/Game/Blueprint/GravityFields/BP_GravityField_Sphere"));
	if (GravityFieldSphereBPClass.Class != nullptr)
	{
		GravityFieldSphereClass = GravityFieldSphereBPClass.Class;
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

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// 델리게이트 바인딩
	OnActorBeginOverlap.AddDynamic(this, &AOPChampion::OnOverlapBegin);
}

void AOPChampion::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		// 블루프린트 클래스로 캐스팅을 시도합니다.
		if (OtherActor->GetClass()->IsChildOf(GravityFieldSphereClass))
		{
			// 캐스팅이 성공한 경우, CurrentGravityField를 설정합니다.(현재 중력 필드)
			CurrentGravityField = OtherActor;

			//GetGravityDirection 함수를 호출합니다. 
			UFunction* GravityDirectionFunction = OtherActor->FindFunction(TEXT("GetGravityDirection"));
			if (GravityDirectionFunction)
			{
				struct FGravityDirectionParams
				{
					FVector GravityDirection;
					AActor* TargetActor;
				};

				FGravityDirectionParams Params;
				Params.TargetActor = this; // Self 참조 전달

				OtherActor->ProcessEvent(GravityDirectionFunction, &Params);

				FVector GravityDirection = Params.GravityDirection;
				GravityDirection.Normalize(); // 벡터를 노멀라이즈하여 방향 벡터의 크기를 1로 설정
				UE_LOG(LogTemp, Warning, TEXT("Gravity Direction: %s"), *GravityDirection.ToString());

				// 캐릭터의 Character Movement Component에서 Set Gravity Direction 함수 호출
				UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
				if (CharacterMovementComponent)
				{
					UFunction* SetGravityDirectionFunction = CharacterMovementComponent->FindFunction(TEXT("SetGravityDirection"));
					if (SetGravityDirectionFunction)
					{
						struct FSetGravityDirectionParams
						{
							FVector GravityDirection;
						};

						FSetGravityDirectionParams SetGravityParams;
						SetGravityParams.GravityDirection = GravityDirection;

						CharacterMovementComponent->ProcessEvent(SetGravityDirectionFunction, &SetGravityParams);

					}
				}
			}
		}
	}
}

void AOPChampion::NotifyActorEndOverlap(AActor* OtherActor)
{
	// Check if OtherActor is valid and not this actor
	if (OtherActor && OtherActor != this)
	{

		// 블루프린트 클래스로 캐스팅을 시도합니다.
		if (OtherActor->GetClass()->IsChildOf(GravityFieldSphereClass))
		{
			//GetGravityDirection 함수를 호출합니다. 
			UFunction* GravityDirectionFunction = OtherActor->FindFunction(TEXT("GetGravityDirection"));
			if (GravityDirectionFunction)
			{
				// If the cast is successful, set CurrentGravityField to the cast object
				CurrentGravityField = OtherActor;

				// Get overlapping actors of class OtherActor
				TArray<AActor*> OverlappingActors;
				GetOverlappingActors(OverlappingActors, OtherActor->GetClass());

				// Process OverlappingActors array as needed
				if (OverlappingActors.Num() > 0)
				{
					// Example: Log the count of overlapping actors
					UE_LOG(LogTemp, Warning, TEXT("Overlapping Gravity Fields Count: %d"), OverlappingActors.Num());
				}
			}
			else
			{
				// If the cast fails, set CurrentGravityField to nullptr or handle as needed
				CurrentGravityField = nullptr; // Or some other default value
			}
		}
	}
}
void AOPChampion::PlayDeadAnimMontage() const
{
	ChampionAnimInstance->Montage_Play(DeadAnimMontage);
}

void AOPChampion::ResetChampionMovement() const
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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

void AOPChampion::SetCustomGravityDirection(FVector NewGravityDirection)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = 0;  // Disable default gravity
		GetCharacterMovement()->AddForce(NewGravityDirection * 980.f * GetCharacterMovement()->Mass); // Apply custom gravity
	}
}

void AOPChampion::SetCustomGravityScale(float NewGravityScale)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = NewGravityScale;
	}
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
