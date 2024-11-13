// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AWBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TSubclassOf<UUserWidget> widgetClass;

	
	
};
