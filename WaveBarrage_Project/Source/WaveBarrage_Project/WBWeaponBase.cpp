// Fill out your copyright notice in the Description page of Project Settings.


#include "WBWeaponBase.h"
#include "Components/SceneComponent.h"
#include "WBPlayerState.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AWBWeaponBase::AWBWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 투사체 스폰 포인트 컴포넌트
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	RootComponent = ProjectileSpawnPoint;


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
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("SpawnProjectile"), 0.02f, true);
}

void AWBWeaponBase::SpawnProjectile()
{
	if (ProjectileClass && OwnerCharacter && ProjectileSpawnPoint)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Cast<APawn>(OwnerCharacter);

		// Get spawn location and rotation from ProjectileSpawnPoint
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		// Spawn the projectile actor
		AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedProjectile)
		{
			// Handle any initialization or effects for the projectile here
			CurProjectileCnt++;
		}
	}
}

