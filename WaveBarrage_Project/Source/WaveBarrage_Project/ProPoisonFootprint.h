// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBProjectileBase.h"
#include "ProPoisonFootprint.generated.h"


class AWBMonsterBase;

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AProPoisonFootprint : public AWBProjectileBase
{
	GENERATED_BODY()


public:
	AProPoisonFootprint();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;



public:

	bool CanCollision = false;

	FTimerHandle FTimerHandle_DamageTick;

	UFUNCTION()
	void DamageTick(AActor* OtherActor);

	TArray<AWBMonsterBase*> OverlappedMonsters;

	// 각 몬스터별 타이머 핸들
	TMap<AActor*, FTimerHandle> TimerHandles;
};