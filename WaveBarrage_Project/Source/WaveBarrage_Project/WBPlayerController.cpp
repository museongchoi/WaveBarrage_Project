// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerController.h"
#include "WBGameState.h"
#include "WBPlayerState.h"

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

void AWBPlayerController::CardClicked()
{
    C2S_SetPlayerReady();
}

void AWBPlayerController::C2S_SetPlayerReady_Implementation()
{
    AWBPlayerState* MyPlayerState = GetPlayerState<AWBPlayerState>();
    if (MyPlayerState)
    {
        MyPlayerState->SetPlayerState(EPlayerState::Ready);
    }

    AWBGameState* MyGameState = GetWorld()->GetGameState<AWBGameState>();
    if (MyGameState)
    {
        MyGameState->CheckAllPlayersReady();
    }
}
