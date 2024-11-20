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


	UFUNCTION(BlueprintCallable)
	void LevelUp();

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastLevelUp();

	UFUNCTION()
	void SpawnMonster(ESpawnType SpawnType, TSubclassOf<AWBMonsterBase> MonsterClass,  int SpawnCount, float x, float y);
	AActor* GetNearPlayer(AActor* Monster);

	UFUNCTION(BlueprintCallable, Category = "Card Selection")
	void ApplyCardEffect(AWBPlayerController* PlayerController, int32 CardIndex);

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Exp;

	UPROPERTY(EditAnywhere)
	TArray<AWBPlayerBase*> Players;

	UPROPERTY(EditAnywhere)
	TArray<AWBMonsterGroup*> MonsterGroups;

	UPROPERTY(EditAnywhere)
	UDataTable* SpawnDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* WeaponDataTable;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	//TSubclassOf<class UWBULobbyWidget> LobbyWidgetClass;
};
