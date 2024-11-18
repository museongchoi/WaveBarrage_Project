// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerController.h"

void AWBPlayerController::ShowCardSelectionWidget()
{
	if (widgetClass)
	{
        if (!CardSelectionWidgetInstance)
        {
            CardSelectionWidgetInstance = CreateWidget<UUserWidget>(this, widgetClass);
            if (CardSelectionWidgetInstance)
            {
                CardSelectionWidgetInstance->AddToViewport();
            }
        }
	}
}
