// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBPlayerBase.h"
#include "WBWeaponBase.generated.h"

class USceneComponent;
class AWBPlayerState;
class AWBPlayerBase;
class AWBProjectileBase;


// 각 무기의 타입을 식별
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WeaponJinx UMETA(DisplayName = "Jinx"),
	WeaponWhirlwind UMETA(DisplayName = "Whirlwind"),
	WeaponPoisonFootprint UMETA(DisplayName = "PoisonFootprint"),
	WeaponBoomerang UMETA(DisplayName = "Boomerang"),
	WeaponCuteLauncher UMETA(DisplayName = "CuteLauncher")
};


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

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	EWeaponType GetWeaponType() const;

	virtual int32 CalculateFinalDamage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType; // 무기 타입을 나타내는 변수

public:

	// 무기 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponLevel;

	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;

	// 스킬 가속
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillAcceleration;

	// 스킬 기본 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolDown;

	// 크리티컬 확율
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	AWBPlayerBase* OwnerCharacter;

	// 발사체
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileClass;

	// 발사체 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProjectileCount;

	// 현재 발사체 개수 (Default : 0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurProjectileCnt;

	// 최대 발사체 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxProjectileCnt;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;

};
