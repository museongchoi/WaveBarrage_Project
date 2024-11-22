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

	UFUNCTION(Server, Reliable)
	void C2S_SpawnWhirlwindBlade();
	void C2S_SpawnWhirlwindBlade_Implementation();



protected:

	virtual void BeginPlay() override;

	virtual void Fire() override;
	
	FTimerHandle FTimerHandle_Fire;

	
	

};
