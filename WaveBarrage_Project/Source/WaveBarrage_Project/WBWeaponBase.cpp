// Fill out your copyright notice in the Description page of Project Settings.


#include "WBWeaponBase.h"
#include "Components/SceneComponent.h"
#include "WBPlayerBase.h"
#include "WBPlayerState.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBProjectileBase.h"


// Sets default values
AWBWeaponBase::AWBWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 투사체 스폰 포인트 컴포넌트
	//ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	//RootComponent = ProjectileSpawnPoint;

	// 기본 무기 타입 설정 (필요에 따라 수정 가능)
	WeaponType = EWeaponType::WeaponJinx;

	// Initialize default values
	OwnerCharacter = nullptr;
	WeaponLevel = 1;
	Damage = 10;
	SkillAcceleration = 0;
	CriticalChance = 0; // Default critical chance is 5%
	ProjectileCount = 1;
	CurProjectileCnt = 0;
	MaxProjectileCnt = 10; // Default maximum projectile count

}

// Called when the game starts or when spawned
void AWBWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerCharacter == nullptr && GetOwner())
	{
		OwnerCharacter = Cast<AWBPlayerBase>(GetOwner());
	}
	
}

// Called every frame
void AWBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWBWeaponBase::Fire()
{
	if (ProjectileClass && OwnerCharacter)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		// 투사체 생성
		AWBProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AWBProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedProjectile)
		{
			// 데미지 설정
			int32 FinalDamage = CalculateFinalDamage();
			SpawnedProjectile->SetDamage(FinalDamage);
		}
	}
}

void AWBWeaponBase::SpawnProjectile()
{

}

EWeaponType AWBWeaponBase::GetWeaponType() const
{
	return WeaponType;
}

int32 AWBWeaponBase::CalculateFinalDamage()
{
	// 무기의 기본 데미지를 사용
	int32 WeaponBaseDamage = Damage;

	// OwnerCharacter의 PlayerState에서 패시브 데미지 증가율 가져오기
	if (OwnerCharacter)
	{
		AWBPlayerState* PlayerState = OwnerCharacter->GetPlayerState<AWBPlayerState>();
		if (PlayerState)
		{
			int32 DamageMultiplier = Damage + ((Damage*PlayerState->Damage)/100);
			return DamageMultiplier;
		}
	}

	return WeaponBaseDamage;
}

