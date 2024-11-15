// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WBGameMode.generated.h"


class AWBPlayerBase;
class AWBMonsterBase;
class AWBMonsterGroup;


UCLASS()
class WAVEBARRAGE_PROJECT_API AWBGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void AddExp(int Value);
	void LevelUp();
	void SetTargetPlayer();
	void SpawnMonsterGroup();

public:
	int Exp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWBMonsterBase> MonsterClass1;

	UPROPERTY()
	TArray<AWBPlayerBase*> Players;

	UPROPERTY()
	TArray<AWBMonsterGroup*> MonsterGroups;
};
