// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBPlayerBase.h"
#include "WBWeaponBase.generated.h"

class USceneComponent;

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
	virtual void Fire();

	UFUNCTION()
	virtual void SpawnProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProjectileCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	AWBPlayerBase* OwnerCharacter;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurProjectileCnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxProjectileCnt;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;



};
