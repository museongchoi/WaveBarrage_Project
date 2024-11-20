// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameState.h"
#include "WBMonsterBase.h"
#include "WBMonsterGroup.h"

void AWBGameState::S2C_MGSetTargetPlayer_Implementation(AWBMonsterGroup* MG, AActor* TargetPlayer)
{
	if (IsValid(MG) && IsValid(TargetPlayer))
	{
		MG->TargetPlayer = TargetPlayer;
	}
}

void AWBGameState::S2C_MBSetTargetPlayer_Implementation(AWBMonsterBase* MB, AActor* TargetPlayer)
{
	MB->SetTargetPlayer(TargetPlayer);
}
