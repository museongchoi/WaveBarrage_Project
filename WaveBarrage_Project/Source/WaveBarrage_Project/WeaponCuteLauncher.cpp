// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCuteLauncher.h"
#include "Kismet/KismetSystemLibrary.h"

AWeaponCuteLauncher::AWeaponCuteLauncher()
{
}

void AWeaponCuteLauncher::BeginPlay()
{
	Super::BeginPlay();
	UKismetSystemLibrary::K2_SetTimer(this, "CuteLauncherAutomaticAiming", 0.01f, true);
	UKismetSystemLibrary::K2_SetTimer(this, "CheckAttack", 2.0f, true);
}

void AWeaponCuteLauncher::Fire()
{
}

void AWeaponCuteLauncher::SpawnProjectile()
{
}

void AWeaponCuteLauncher::CheckAttack()
{
}

void AWeaponCuteLauncher::CuteLauncherAutomaticAiming()
{
}
