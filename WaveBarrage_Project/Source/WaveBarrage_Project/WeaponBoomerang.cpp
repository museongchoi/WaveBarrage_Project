// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBoomerang.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "WBPlayerState.h"
#include "ProBoomerang.h"

AWeaponBoomerang::AWeaponBoomerang()
{
	SpawnDelay = UKismetMathLibrary::RandomFloatInRange(0.033f, 0.528f);
	WeaponType = EWeaponType::WeaponBoomerang;

	WeaponLevel = 1;
	CoolDown = 6.0f;
	Damage = 40;
	ProjectileCount = 1;

	SetReplicates(true);
	SetReplicateMovement(true);

}

void AWeaponBoomerang::BeginPlay()
{
	Super::BeginPlay();
	

	

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_Fire, this, &AWeaponBoomerang::Fire, CoolDown, true);
	}

}

void AWeaponBoomerang::Fire()
{
	Super::Fire();
	if (CurProjectileCnt == 0)
	{
		CalculateAttackStatus();

		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_SpawnProjectile, this, &AWeaponBoomerang::SpawnProjectile, SpawnDelay, true);
	}

	

}

void AWeaponBoomerang::SpawnProjectile()
{
	Super::SpawnProjectile();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();


	if (ProjectileClass)
	{
		FTransform SpawnTransform = FTransform (ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation());
		AProBoomerang* SpawnedProjectile = GetWorld()->SpawnActor<AProBoomerang>(ProjectileClass, SpawnTransform,SpawnParams);

		CurProjectileCnt++;

		int32 FinalDamage = CalculateFinalDamage();
		CanCritialAttack(FinalDamage);
		SpawnedProjectile->SetDamage(FinalDamage);
		SpawnedProjectile->CanCollision = true;
		SpawnedProjectile->SetReplicates(true);

		if (CurProjectileCnt >= MaxProjectileCnt)
		{
			CurProjectileCnt = 0;
			GetWorld()->GetTimerManager().ClearTimer(FTimerHandle_SpawnProjectile);
			
		}
		
	}
}
