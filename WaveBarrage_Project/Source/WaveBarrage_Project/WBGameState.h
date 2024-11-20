// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WBGameState.generated.h"

/**
 * 
 */
class AWBMonsterGroup;
class AWBMonsterBase;

UCLASS()
class WAVEBARRAGE_PROJECT_API AWBGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void S2C_MGSetTargetPlayer(AWBMonsterGroup* MG, AActor* TargetPlayer);
	void S2C_MGSetTargetPlayer_Implementation(AWBMonsterGroup* MG, AActor* TargetPlayer);

	UFUNCTION(NetMulticast, Reliable)
	void S2C_MBSetTargetPlayer(AWBMonsterBase* MB, AActor* TargetPlayer);
	void S2C_MBSetTargetPlayer_Implementation(AWBMonsterBase* MB, AActor* TargetPlayer);


	UFUNCTION()
	void CheckAllPlayersReady();
	


};
