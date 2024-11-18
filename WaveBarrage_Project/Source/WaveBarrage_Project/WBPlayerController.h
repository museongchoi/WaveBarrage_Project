// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WBPlayerController.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AWBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowCardSelectionWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> widgetClass;

	UUserWidget* CardSelectionWidgetInstance;
};
