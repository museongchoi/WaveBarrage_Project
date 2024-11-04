// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_PGN.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"


// Sets default values
ACharacter_PGN::ACharacter_PGN()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<UArrowComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);

	IMC = CreateDefaultSubobject<UInputMappingContext>("IMC");

	

	IA_Move = CreateDefaultSubobject<UInputAction>("IA_Move");

	


}

// Called when the game starts or when spawned
void ACharacter_PGN::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (IsValid(PC))
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer());

		if (IsValid(LocalPlayer))
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = Cast<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>());
			if (IsValid(Subsystem))
			{
				Subsystem->AddMappingContext(IMC, 0);
				UE_LOG(LogTemp, Warning, TEXT("InputAction"));
			}
		}
	}

}

// Called every frame
void ACharacter_PGN::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacter_PGN::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACharacter_PGN::Move);
}

void ACharacter_PGN::Move(const FInputActionValue &Action)
{
	UE_LOG(LogTemp, Warning, TEXT("InputAction"));
}

void ACharacter_PGN::Look(const FInputActionValue &Action)
{
}

