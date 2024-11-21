// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBWeaponBase.h"
#include "WeaponBoomerang.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AWeaponBoomerang : public AWBWeaponBase
{
	GENERATED_BODY()

public:
	AWeaponBoomerang();

protected:
	virtual void BeginPlay() override;

	virtual void Fire() override;

	virtual void SpawnProjectile() override;

	float SpawnDelay = 0.0f;

	FTimerHandle FTimerHandle_Fire;
	FTimerHandle FTimerHandle_SpawnProjectile;
};
