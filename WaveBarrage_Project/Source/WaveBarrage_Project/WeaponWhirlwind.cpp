// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponWhirlwind.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProWhirlwindBlade.h"


AWeaponWhirlwind::AWeaponWhirlwind()
{
	WeaponType = EWeaponType::WeaponWhirlwind;
	WeaponLevel = 1;
	CoolDown = 6.0f;
}

void AWeaponWhirlwind::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(FTimerHandle_Fire, this, &AWeaponWhirlwind::Fire, CoolDown,true);
	UE_LOG(LogTemp, Warning, TEXT("WhirlwindSpawn"));
}

void AWeaponWhirlwind::Fire()
{
	FActorSpawnParameters SpawnPara;
	UE_LOG(LogTemp, Warning, TEXT("WhirlwindFire"));
	switch (WeaponLevel)
	{
		case 1:
		case 2:
			for (int i = 1; i <= 2; i++)
			{
				AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass);
				{
					WhirlwindBlade->SpawnLocationByRadians = 180 * i;
					

				}
			}
			break;
		case 3:
		case 4:
			for (int i = 1; i <= 4; i++)
			{
				AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass);
				{
					WhirlwindBlade->SpawnLocationByRadians = 90 * i;
				

				}
			}
			break;
		case 5:
			for (int i = 1; i <= 6; i++)
			{
				AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass);
				{
					WhirlwindBlade->SpawnLocationByRadians = 60 * i;
					
					
				}
				
			}
			break;
	}

}
