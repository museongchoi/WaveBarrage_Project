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

	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	bool CanCollision = false;

public:


};