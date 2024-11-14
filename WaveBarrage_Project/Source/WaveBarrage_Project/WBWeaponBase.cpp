// Fill out your copyright notice in the Description page of Project Settings.


#include "WBWeaponBase.h"
#include "Components/SceneComponent.h"

// Sets default values
AWBWeaponBase::AWBWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	RootComponent = ProjectileSpawnPoint;

}

// Called when the game starts or when spawned
void AWBWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWBWeaponBase::Fire()
{
}

void AWBWeaponBase::SpawnProjectile()
{
}

