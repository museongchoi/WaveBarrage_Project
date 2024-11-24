// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FWBPlayerST.h"
#include "WBGameState.generated.h"

/**
 * 
 */
class AWBMonsterGroup;
class AWBMonsterBase;
class AWBPlayerState;
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerHPChanged, int32, PlayerID, int32, NewHP);
UCLASS()
class WAVEBARRAGE_PROJECT_API AWBGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AWBGameState();

	UFUNCTION(NetMulticast, Reliable)
	void S2C_MGSetTargetPlayer(AWBMonsterGroup* MG, AActor* TargetPlayer);
	void S2C_MGSetTargetPlayer_Implementation(AWBMonsterGroup* MG, AActor* TargetPlayer);

	UFUNCTION(NetMulticast, Reliable)
	void S2C_MBSetTargetPlayer(AWBMonsterBase* MB, AActor* TargetPlayer);
	void S2C_MBSetTargetPlayer_Implementation(AWBMonsterBase* MB, AActor* TargetPlayer);

	UFUNCTION()
	void CheckAllPlayersReady();
	
	// 플레이어 HP 배열
	UPROPERTY(ReplicatedUsing = OnRep_PlayerStates)
	TArray<FWBPlayerST> PlayerStates;

	UFUNCTION(BlueprintCallable)
	int32 AssignPlayerID();

	// HP 변경 시 호출되는 함수
	UFUNCTION()
	void OnRep_PlayerStates();

	// 플레이어 HP 업데이트 함수
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerHP(int32 PlayerID, int32 Damage);

	UFUNCTION(BlueprintCallable)
	// 게임 종료 조건 확인
	void EndGame();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//// 플레이어 HP 변화 델리게이트
	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FOnPlayerHPChanged OnPlayerHPChanged;
};
