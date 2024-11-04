// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Character_PGN.generated.h"

class UArrowComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class WAVEBARRAGE_PROJECT_API ACharacter_PGN : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_PGN();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr < UArrowComponent > CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> IA_Look;

	void Move(const FInputActionValue &Action);

	void Look(const FInputActionValue &Action);
};
