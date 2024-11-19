// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBProjectileBase.h"
#include "ProPoisonFootprint.generated.h"

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

public:

	UFUNCTION()
	void AddDamage();
};