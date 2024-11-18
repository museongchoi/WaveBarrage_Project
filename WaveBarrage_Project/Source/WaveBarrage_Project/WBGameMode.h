// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WBSapwnDataTable.h"
#include "WBGameMode.generated.h"


class AWBPlayerBase;
class AWBMonsterBase;
class AWBMonsterGroup;


UCLASS()
class WAVEBARRAGE_PROJECT_API AWBGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AWBGameMode();
	virtual void BeginPlay() override;
	void AddExp(int Value);
	void LevelUp();
	void SetTargetPlayer();
	UFUNCTION()
	void SpawnMonster(ESpawnType SpawnType, TSubclassOf<AWBMonsterBase> MonsterClass,  int SpawnCount, float x, float y);
	AActor* GetNearPlayer(AActor* Monster);

public:
	int Exp;

	UPROPERTY(EditAnywhere)
	TArray<AWBPlayerBase*> Players;

	UPROPERTY(EditAnywhere)
	TArray<AWBMonsterGroup*> MonsterGroups;

	UPROPERTY(EditAnywhere)
	UDataTable* SpawnDataTable;

	UPROPERTY(EditAnywhere)
	UDataTable* WeaponDataTable;
};
