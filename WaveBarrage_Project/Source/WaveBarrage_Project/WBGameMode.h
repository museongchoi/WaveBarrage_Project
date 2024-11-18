// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WBSapwnDataTable.h"
#include "WBWeaponDataTable.h"
#include "WBGameMode.generated.h"

class AWBPlayerState;
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

	UFUNCTION(BlueprintCallable, Category = "Card Selection")
	void ApplyCardEffect(AWBPlayerController* PlayerController, int32 CardIndex);

public:
	int Exp;

	UPROPERTY(EditAnywhere)
	TArray<AWBPlayerBase*> Players;

	UPROPERTY(EditAnywhere)
	TArray<AWBMonsterGroup*> MonsterGroups;

	UPROPERTY(EditAnywhere)
	UDataTable* SpawnDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* WeaponDataTable;


};
