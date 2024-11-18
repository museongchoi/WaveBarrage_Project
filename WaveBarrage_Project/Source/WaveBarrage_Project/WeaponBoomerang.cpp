// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBoomerang.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "WBPlayerState.h"

AWeaponBoomerang::AWeaponBoomerang()
{
	SpawnDelay = UKismetMathLibrary::RandomFloatInRange(0.033f, 0.528f);
	WeaponType = EWeaponType::WeaponBoomerang;
}

void AWeaponBoomerang::BeginPlay()
{
	Super::BeginPlay();
	UKismetSystemLibrary::K2_SetTimer(this, "Fire", 4.0f, true);

	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (MyCharacter)
	{
		AWBPlayerState* MyPs = Cast<AWBPlayerState>(MyCharacter->GetPlayerState());
		if (MyPs)
		{
			MaxProjectileCnt = ProjectileCount + MyPs->ProjectileCounts;
		}
	}

}

void AWeaponBoomerang::Fire()
{
	Super::Fire();
	if (CurProjectileCnt == 0)
	{
		UKismetSystemLibrary::K2_SetTimer(this, "SpawnProjectile", SpawnDelay, true);
	}
}

void AWeaponBoomerang::SpawnProjectile()
{
	Super::SpawnProjectile();
	if (ProjectileClass)
	{
		FTransform SpawnTransform = FTransform (ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation());
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform);

		CurProjectileCnt++;
		if (CurProjectileCnt >= MaxProjectileCnt)
		{
			UKismetSystemLibrary::K2_ClearTimer(this, "SpawnProjectile");
			CurProjectileCnt = 0;
		}
		
	}
}
