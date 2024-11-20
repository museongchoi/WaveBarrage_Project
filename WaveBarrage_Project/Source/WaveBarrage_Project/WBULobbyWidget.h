// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBULobbyWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API UWBULobbyWidget : public UUserWidget
{
	GENERATED_BODY()


public:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnCreateSessionClicked();

    UFUNCTION()
    void OnJoinSessionClicked();


public:
    UPROPERTY(meta = (BindWidget))
    UButton* CreateSessionButton;

    UPROPERTY(meta = (BindWidget))
    UButton* JoinSessionButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ProjectNameText;

    UPROPERTY(meta = (BindWidget))
    UImage* GameImage;
	
};
