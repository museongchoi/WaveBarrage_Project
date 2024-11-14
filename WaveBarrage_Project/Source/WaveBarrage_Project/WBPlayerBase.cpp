// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AWBPlayerBase::AWBPlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->MaxAcceleration = 1000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->TargetArmLength = 4000.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 55.0f;


	Box1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box1"));
	Box1->SetupAttachment(RootComponent);

	Box2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box2"));
	Box2->SetupAttachment(RootComponent);

	Box3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box3"));
	Box3->SetupAttachment(RootComponent);

	Box4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box4"));
	Box4->SetupAttachment(RootComponent);

	Box5 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box5"));
	Box5->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWBPlayerBase::BeginPlay()
{
	Super::BeginPlay();
	ConfigureInputMapping();
}

// Called every frame
void AWBPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWBPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Move)
		{
			EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AWBPlayerBase::Move);
		}
		if (IA_Skill_E)
		{
			EnhancedInputComponent->BindAction(IA_Skill_E, ETriggerEvent::Started, this, &AWBPlayerBase::SkillE);
		}
		if (IA_Skill_R)
		{
			EnhancedInputComponent->BindAction(IA_Skill_R, ETriggerEvent::Started, this, &AWBPlayerBase::SkillR);
		}
		if (IA_AutoMode)
		{
			EnhancedInputComponent->BindAction(IA_AutoMode, ETriggerEvent::Started, this, &AWBPlayerBase::ToggleAutoMode);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AWBPlayerBase::ConfigureInputMapping()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (IsValid(PlayerController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMapping)
			{
				Subsystem->AddMappingContext(InputMapping, 0);
			}
		}
	}
}

void AWBPlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator MyRotation = GetControlRotation();
		const FRotator YawRotation(0, MyRotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWBPlayerBase::SkillE()
{
}

void AWBPlayerBase::SkillR()
{
}

void AWBPlayerBase::ToggleAutoMode()
{
}

