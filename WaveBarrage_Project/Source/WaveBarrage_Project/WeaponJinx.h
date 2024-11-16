// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBWeaponBase.h"
#include "WeaponJinx.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AWeaponJinx : public AWBWeaponBase
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;

	virtual void SpawnProjectile() override;
};
