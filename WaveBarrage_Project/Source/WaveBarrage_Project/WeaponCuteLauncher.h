// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBWeaponBase.h"
#include "WeaponCuteLauncher.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AWeaponCuteLauncher : public AWBWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeaponCuteLauncher();

protected:

	virtual void BeginPlay() override;

	virtual void Fire() override;

	virtual void SpawnProjectile() override;

	void CuteLauncherAutomaticAiming();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ClosestDistance;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> ClosestEnemy;

	FTimerHandle FTimerHandle_AutomaticAiming;
	FTimerHandle FTimerHandle_AttackFire;

};
