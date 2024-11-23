// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBMonsterGroup.generated.h"

class AWBMonsterBase;

UCLASS()
class WAVEBARRAGE_PROJECT_API AWBMonsterGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWBMonsterGroup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnMonster();
	void SpawnRandomPositionMonster();
	void SpawnCirclePositionMonster();
	void RemoveMonster(AWBMonsterBase* Monster);
	void UpdateTargetPlayer();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TArray<AWBMonsterBase*> Monsters;

	UPROPERTY(VisibleAnywhere)
	AActor* TargetPlayer;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AWBMonsterBase> MonsterClass;

	UPROPERTY(VisibleAnywhere)
	int SpawnCount;

	bool SpawnEnd = false;
	UPROPERTY(EditAnywhere)
	bool IsNotUpdate = false;
};
