// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBWeaponBase.generated.h"

UCLASS()
class WAVEBARRAGE_PROJECT_API AWBWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWBWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void dFire();

	UPROPERTY()
	int32 WeaponLevel;

	UPROPERTY()
	int32 Damage;

	UPROPERTY()
	int32 SkillAcceleration;

	UPROPERTY()
	int32 CriticalChance;

	UPROPERTY()
	int32 ProjectileCount;

	UPROPERTY()
	AActor* OwnerCharacter;

	UPROPERTY()
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY()
	int32 CurProjectileCnt;

	UPROPERTY()
	int32 MaxProjectileCnt;
};
