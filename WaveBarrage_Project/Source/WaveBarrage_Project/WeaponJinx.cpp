// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponJinx.h"
#include "WBPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WBProjectileBase.h"

AWeaponJinx::AWeaponJinx()
{
	// 투사체 스폰 포인트 컴포넌트
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	RootComponent = ProjectileSpawnPoint;

	WeaponType = EWeaponType::WeaponJinx;
	WeaponLevel = 1;
	Damage = 20;
	ProjectileCount = 1;
	MaxProjectileCnt = 10;
}

void AWeaponJinx::BeginPlay()
{
	Super::BeginPlay();

}

void AWeaponJinx::Fire()
{

	// Get projectile count from player state and update max projectile count
	if (OwnerCharacter)
	{

		AWBPlayerState* PlayerState = Cast<AWBPlayerState>(OwnerCharacter->GetPlayerState());
		if (PlayerState)
		{
			//UE_LOG(LogTemp, Error, TEXT("5. AWeaponJinx Fire Check!!!!!!!"));

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
		AWBProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AWBProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedProjectile)
		{
			// Increment current projectile count
			CurProjectileCnt++;
			//UE_LOG(LogTemp, Error, TEXT("%d"), CurProjectileCnt);
			int32 FinalDamage = CalculateFinalDamage();
			SpawnedProjectile->SetDamage(FinalDamage);

		}
	}

	// Check if the current projectile count has reached the max
	if (CurProjectileCnt >= MaxProjectileCnt)
	{
		UE_LOG(LogTemp, Error, TEXT("%d"), MaxProjectileCnt);

		// Clear the timer to stop spawning projectiles
		UKismetSystemLibrary::K2_ClearTimer(this, TEXT("SpawnProjectile"));

		// Reset current projectile count
		CurProjectileCnt = 0;

		UE_LOG(LogTemp, Error, TEXT("6. bIsAttacking false Check!!!!!!!"));
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		OwnerCharacter->GetMesh()->SetWorldRotation(FRotator(0.0f, OwnerCharacter->GetActorRotation().Yaw - 90.0f, 0.0f));

	}
}
