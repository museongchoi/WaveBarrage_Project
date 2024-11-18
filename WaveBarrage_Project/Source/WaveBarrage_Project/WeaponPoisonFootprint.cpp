// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPoisonFootprint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProPoisonFootprint.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AWeaponPoisonFootprint::AWeaponPoisonFootprint()
{
	WeaponType = EWeaponType::WeaponPoisonFootprint;
}

void AWeaponPoisonFootprint::BeginPlay()
{
	Super::BeginPlay();
	
	UKismetSystemLibrary::K2_SetTimer(this, "Fire", 2.0f, true);
}

void AWeaponPoisonFootprint::Fire()
{	
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector SPawnLocation = FVector(MyCharacter->GetActorLocation().X, MyCharacter->GetActorLocation().Y, 0);
	FTransform SpawnTransform = FTransform(MyCharacter->GetActorRotation(),SPawnLocation);

	if (ProjectileClass)
	{
		GetWorld()->SpawnActor<AProPoisonFootprint>(ProjectileClass,SpawnTransform);
	}
}
