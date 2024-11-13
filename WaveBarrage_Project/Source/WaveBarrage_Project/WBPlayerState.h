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
	UPROPERTY()
	int32 Damage;

	UPROPERTY()
	int32 CriticalHitChance;

	UPROPERTY()
	int32 SkillAcceleration;

	UPROPERTY()
	int32 ProjectileCounts;

	UPROPERTY()
	int32 MovementSpeed;

	UPROPERTY()
	int32 MaxHealth;

	UPROPERTY()
	int32 Armor;

	UPROPERTY()
	int32 StaminaRegeneration;

	UPROPERTY()
	int32 RangeSize;

	UPROPERTY()
	int32 Duration;

	UPROPERTY()
	int32 AcquisitionRadius;

	UPROPERTY()
	int32 ExperienceBonus;

	UPROPERTY()
	TMap<FString, int16> ItemLevel;
};
