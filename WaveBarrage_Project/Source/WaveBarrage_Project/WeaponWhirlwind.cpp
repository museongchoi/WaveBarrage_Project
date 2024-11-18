// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponWhirlwind.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProWhirlwindBlade.h"


AWeaponWhirlwind::AWeaponWhirlwind()
{
	WeaponType = EWeaponType::WeaponWhirlwind;

}

void AWeaponWhirlwind::BeginPlay()
{
	Super::BeginPlay();
	UKismetSystemLibrary::K2_SetTimer(this, "Fire", 4.0f, true);
}

void AWeaponWhirlwind::Fire()
{
	FActorSpawnParameters SpawnPara;
	
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
