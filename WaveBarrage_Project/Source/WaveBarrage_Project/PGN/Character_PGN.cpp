// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_PGN.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACharacter_PGN::ACharacter_PGN()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<UArrowComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);


	


}

// Called when the game starts or when spawned
void ACharacter_PGN::BeginPlay()
{
	Super::BeginPlay();
	
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

}

