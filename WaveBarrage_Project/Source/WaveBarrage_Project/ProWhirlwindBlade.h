// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBProjectileBase.h"
#include "ProWhirlwindBlade.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AProWhirlwindBlade : public AWBProjectileBase
{
	GENERATED_BODY()

public:
	AProWhirlwindBlade();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitSpeed = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitRadius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurAngle = 0.0f;

	FVector Center;

public:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"));
	float SpawnLocationByRadians=0.0f;

	bool CanCollision = false;

	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
