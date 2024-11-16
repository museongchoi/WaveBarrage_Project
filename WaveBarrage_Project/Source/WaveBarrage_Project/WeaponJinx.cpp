// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponJinx.h"
#include "WBPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBPlayerBase.h"

void AWeaponJinx::Fire()
{
	//UE_LOG(LogTemp, Error, TEXT("AWeaponJinx Fire Check!!!!!!!"));

	// Get projectile count from player state and update max projectile count
	if (OwnerCharacter)
	{
		AWBPlayerState* PlayerState = Cast<AWBPlayerState>(OwnerCharacter->GetPlayerState());
		if (PlayerState)
		{
			MaxProjectileCnt = ProjectileCount + PlayerState->ProjectileCounts;
		}
	}

	// Set timer to repeatedly call SpawnProjectile
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("SpawnProjectile"), 0.02f, true);
}

void AWeaponJinx::SpawnProjectile()
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
			// Increment current projectile count
			CurProjectileCnt++;
		}
	}

	// Check if the current projectile count has reached the max
	if (CurProjectileCnt >= MaxProjectileCnt)
	{
		// Clear the timer to stop spawning projectiles
		UKismetSystemLibrary::K2_ClearTimer(this, TEXT("SpawnProjectile"));

		// Reset current projectile count
		CurProjectileCnt = 0;

		// Set owner character's IsAttacking to false and call AttackFire function
		AWBPlayerBase* PlayerCharacter = Cast<AWBPlayerBase>(OwnerCharacter);
		if (PlayerCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("bIsAttacking false Check!!!!!!!"));

			PlayerCharacter->bIsAttacking = false;
			PlayerCharacter->AttackFire();
		}
	}
}
