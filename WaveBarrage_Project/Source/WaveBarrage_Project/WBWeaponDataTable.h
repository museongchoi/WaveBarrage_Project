// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WBWeaponBase.h"
#include "WBWeaponDataTable.generated.h"

/**
 * 
 */
USTRUCT()
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// 무기 레벨
	UPROPERTY(EditAnywhere)
	int32 WeaponLevel;

	// 데미지
	UPROPERTY(EditAnywhere)
	int32 Damage;

	// 스킬 가속
	UPROPERTY(EditAnywhere)
	int32 SkillAcceleration;

	// 스킬 기본 속도
	UPROPERTY(EditAnywhere)
	float CoolDown;

	// 크리티컬 확율
	UPROPERTY(EditAnywhere)
	int32 CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,meta = (ExposeOnSpawn = "true"))
	AWBPlayerBase* OwnerCharacter;

	// 발사체
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

	// 발사체 개수
	UPROPERTY(EditAnywhere)
	int32 ProjectileCount;

	// 현재 발사체 개수 (Default : 0)
	UPROPERTY(EditAnywhere)
	int32 CurProjectileCnt;

	// 최대 발사체 개수
	UPROPERTY(EditAnywhere)
	int32 MaxProjectileCnt;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;
};

UCLASS()
class WAVEBARRAGE_PROJECT_API UWBWeaponDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
