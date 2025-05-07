// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssignmentCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerInfo.h"
#include "PlayerUI.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AIPerceptionSystem.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAssignmentCharacter

AAssignmentCharacter::AAssignmentCharacter() : grabActor(nullptr), grabComp(nullptr)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	physicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	health = 100.0f;
}

void AAssignmentCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(damageTick, this, &AAssignmentCharacter::Damage, 1.0f, true);
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(GetWorld(), UAISense_Sight::StaticClass(), this);
}

void AAssignmentCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// kill
	PlayerInfo::GetInstance()->health = 0;
}

void AAssignmentCharacter::Damage()
{
	if (PlayerInfo::GetInstance()->isInitialized && PlayerInfo::GetInstance()->GetGameState() == EUserPlayStateEnum::PLAY)
	{
		PlayerInfo::GetInstance()->health -= 1.0f;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAssignmentCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// NO Jumping!!!!
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAssignmentCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAssignmentCharacter::Look);

		// Drop Item
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AAssignmentCharacter::dropObject);

		// Throw Item
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &AAssignmentCharacter::throwObject);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAssignmentCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAssignmentCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// grab object
void AAssignmentCharacter::OnGrab(AActor* a, UPrimitiveComponent* hitComp)
{
	if (grabActor == nullptr && hitComp->IsSimulatingPhysics())
	{
		grabActor = Cast<AActor>(a);
		grabComp = hitComp;
		compCollisionType = grabComp->GetCollisionEnabled();
		grabComp->SetSimulatePhysics(false);
		grabComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		physicsHandle->GrabComponentAtLocation(grabComp, FName("None"), grabActor->GetActorLocation());

		if (GetMesh()->GetSocketByName(FName("CollSocket")) != NULL)
		{
			grabActor->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), TEXT("CollSocket"));
		}
	}
}

void AAssignmentCharacter::dropObject()
{
	if (grabActor != nullptr)
	{
		detachObject();
	}
}
void AAssignmentCharacter::throwObject()
{
	
	if (grabActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Throwing object! %s"), *grabActor->GetName());
		detachObject();
		grabComp->AddImpulse(GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Vector() * 2000.0f, NAME_None, true);
	}
}

void AAssignmentCharacter::detachObject()
{
	//UE_LOG(LogTemp, Warning, TEXT("Detaching object! %s"), grabActor->GetName());
	grabActor->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	grabComp->SetSimulatePhysics(true);
	grabComp->SetCollisionEnabled(compCollisionType);
	// re-enabled in the tick of the object dropped
	grabComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	grabActor = nullptr;


	// if theyve dropped something, and it happened to be the target, set it to false. Better this than checking if its the target everytime
	PlayerInfo::GetInstance()->targetGet = false;
}