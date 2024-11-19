// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBProjectileBase.h"
#include "ProCuteLauncher.generated.h"

class UProjectileMovementComponent;
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

	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
};
