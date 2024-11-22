// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPoisonFootprint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProPoisonFootprint.h"


AWeaponPoisonFootprint::AWeaponPoisonFootprint()
{
	WeaponType = EWeaponType::WeaponPoisonFootprint;
	WeaponLevel = 1;
	CoolDown = 3.0f;
	Damage = 15;
	ProjectileCount = 0;

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AWeaponPoisonFootprint::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_Fire, this, &AWeaponPoisonFootprint::Fire, CoolDown, true);
	}
}

void AWeaponPoisonFootprint::Fire()
{	
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y,0);

	if (ProjectileClass)
	{
		AProPoisonFootprint* SpawnedProjectile = GetWorld()->SpawnActor<AProPoisonFootprint>(ProjectileClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

		int32 FinalDamage = CalculateFinalDamage();
		SpawnedProjectile->SetDamage(FinalDamage);
		SpawnedProjectile->CanCollision = true;
		SpawnedProjectile->SetReplicates(true);
	}
}
