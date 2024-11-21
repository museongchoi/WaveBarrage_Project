// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBWeaponBase.h"
#include "WeaponWhirlwind.generated.h"

/**
 * 
 */

UCLASS()
class WAVEBARRAGE_PROJECT_API AWeaponWhirlwind : public AWBWeaponBase
{
	GENERATED_BODY()

public:

	AWeaponWhirlwind();

protected:

	virtual void BeginPlay() override;

	virtual void Fire() override;
	
	FTimerHandle FTimerHandle_Fire;


};
