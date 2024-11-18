// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AWBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalHitChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProjectileCounts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StaminaRegeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RangeSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AcquisitionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExperienceBonus;

	UPROPERTY(EditAnywhere)
	TMap<FString, int16> ItemLevel;
};
