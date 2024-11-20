// Fill out your copyright notice in the Description page of Project Settings.


#include "WBULobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UWBULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CreateSessionButton)
    {
        CreateSessionButton->OnClicked.AddDynamic(this, &UWBULobbyWidget::OnCreateSessionClicked);
    }

    if (JoinSessionButton)
    {
        JoinSessionButton->OnClicked.AddDynamic(this, &UWBULobbyWidget::OnJoinSessionClicked);
    }

    if (ProjectNameText)
    {
        ProjectNameText->SetText(FText::FromString(TEXT("Wave Barrage Project")));
    }

    // 추가적으로, GameImage에 대한 설정도 할 수 있습니다 (이미지를 설정하는 코드)
}

void UWBULobbyWidget::OnCreateSessionClicked()
{
    // 세션 생성 로직 추가
    UE_LOG(LogTemp, Warning, TEXT("Create Session Clicked"));
}

void UWBULobbyWidget::OnJoinSessionClicked()
{
    // 세션 조인 로직 추가
    UE_LOG(LogTemp, Warning, TEXT("Join Session Clicked"));
}
