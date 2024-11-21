// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBWeaponBase.h"
#include "WeaponPoisonFootprint.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AWeaponPoisonFootprint : public AWBWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeaponPoisonFootprint();

protected:
	virtual void BeginPlay() override;

	virtual void Fire() override;

	FTimerHandle FTimerHandle_Fire;


};
