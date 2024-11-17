// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBProjectileBase.h"
#include "ProCuteLauncher.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AProCuteLauncher : public AWBProjectileBase
{
	GENERATED_BODY()
	
public:
	AProCuteLauncher();

	virtual void BeginPlay() override;
};
