// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WBMonsterBase.h"
#include "WBSapwnDataTable.generated.h"


UENUM()
enum class ESpawnType : uint8
{
	Normal = 0,
	Minion = 1,
	Circle = 2,
	Elite = 3,
	Boss = 4
};

USTRUCT()
struct FSpawnData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float SpawnTime;

	UPROPERTY(EditAnywhere)
	ESpawnType SpawnType = ESpawnType::Normal;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWBMonsterBase> MonsterClass;

	UPROPERTY(EditAnywhere)
	float x = 0;

	UPROPERTY(EditAnywhere)
	float y = 0;

	UPROPERTY(EditAnywhere)
	int SpawnCount = 1;

};


UCLASS()
class WAVEBARRAGE_PROJECT_API UWBSapwnDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
