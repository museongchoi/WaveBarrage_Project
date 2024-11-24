// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameState.h"
#include "WBMonsterBase.h"
#include "WBMonsterGroup.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "WBPlayerState.h"
#include "WBGameMode.h"
#include "FWBPlayerST.h"
#include "Engine/Engine.h"

AWBGameState::AWBGameState()
{
    // 서버에서만 초기화
    if (HasAuthority())
    {
        // 초기화할 플레이어 수에 따라 PlayerStates 배열을 채웁니다.
        // 예를 들어, 최대 4명의 플레이어라면:
        PlayerStates.SetNum(4);
        for (int32 i = 0; i < PlayerStates.Num(); ++i)
        {
            PlayerStates[i] = FWBPlayerST(0, 1000);
        }
    }

}

void AWBGameState::BeginPlay()
{
    Super::BeginPlay();
}

void AWBGameState::S2C_MGSetTargetPlayer_Implementation(AWBMonsterGroup* MG, AActor* TargetPlayer)
{
	if (IsValid(MG) && IsValid(TargetPlayer))
	{
		MG->TargetPlayer = TargetPlayer;
	}
}

void AWBGameState::S2C_MBSetTargetPlayer_Implementation(AWBMonsterBase* MB, AActor* TargetPlayer)
{
    if (IsValid(MB) && IsValid(TargetPlayer))
    {
        MB->SetTargetPlayer(TargetPlayer);
    }
}

void AWBGameState::CheckAllPlayersReady()
{
    for (APlayerState* PlayerState : PlayerArray)
    {
        AWBPlayerState* MyPlayerState = Cast<AWBPlayerState>(PlayerState);
        if (MyPlayerState && MyPlayerState->CurrentState != EPlayerState::Ready)
        {
            return; // 한 명이라도 Ready가 아니면 리턴
        }
    }
        // 모든 플레이어가 준비되었을 경우
        UGameplayStatics::SetGamePaused(GetWorld(), false);
       
        for (APlayerState* PlayerState : PlayerArray)
        {
            AWBPlayerState* MyPlayerState = Cast<AWBPlayerState>(PlayerState);
            MyPlayerState->SetPlayerState(EPlayerState::Waiting);
        }
}

int32 AWBGameState::AssignPlayerID()
{
    for (int32 i = 0; i < PlayerStates.Num(); ++i)
    {
        if (PlayerStates[i].PlayerID == 0)
        {
            PlayerStates[i].PlayerID = i + 1; // 1부터 시작
            PlayerStates[i].HP = 1000;
            return PlayerStates[i].PlayerID;
        }
    }
    return -1; // 할당할 수 없는 경우
}

void AWBGameState::OnRep_PlayerStates()
{
    // PlayerStates가 변경되었을 때 클라이언트에서 처리할 로직을 여기에 작성
    // 예: UI 업데이트 등
    for (const auto& PlayerState : PlayerStates)
    {
        UE_LOG(LogTemp, Log, TEXT("PlayerID: %d, HP: %d"), PlayerState.PlayerID, PlayerState.HP);
    }
}

void AWBGameState::UpdatePlayerHP(int32 PlayerID, int32 Damage)
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerHP not Server."));
        return;
    }

    for (auto& PlayerState : PlayerStates)
    {
        if (PlayerState.PlayerID == PlayerID)
        {
            PlayerState.HP -= Damage;
            PlayerState.HP = FMath::Max(PlayerState.HP, 0);
            UE_LOG(LogTemp, Warning, TEXT("PlayerID %d took %d damage, HP now %d"), PlayerID, Damage, PlayerState.HP);

            if (PlayerState.HP <= 0)
            {
                EndGame();
            }

            // 배열 전체를 다시 복제하도록 MarkItemDirty를 호출하지 않고, PlayerStates 배열을 수정했기 때문에 OnRep_PlayerStates가 호출됩니다.
            // 따라서 별도의 복제 호출은 필요하지 않습니다.
            break;
        }
    }
}

void AWBGameState::EndGame()
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("EndGame Client Call Err."));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("EndGame."));

    // 게임 종료 로직 구현
    // 예: 게임 모드에 종료 신호 보내기, 결과 화면으로 전환 등
    // 예시로 모든 플레이어에게 게임 종료 메시지를 브로드캐스트합니다.
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC)
        {
            // 클라이언트에게 게임 종료 알림을 보냅니다.
            // Blueprint나 다른 방법으로 클라이언트 UI를 업데이트할 수 있습니다.
            PC->ClientMessage(TEXT("Game End!!!."));
        }
    }
}

void AWBGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AWBGameState, PlayerStates);
}

void AWBGameState::UpdateAllPlayerHPHell()
{

}
