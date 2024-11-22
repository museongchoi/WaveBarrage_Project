// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPoisonFootprint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProPoisonFootprint.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
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
	
	GetWorld()->GetTimerManager().SetTimer(FTimerHandle_Fire, this, &AWeaponPoisonFootprint::Fire, CoolDown, true);
}

void AWeaponPoisonFootprint::Fire()
{	
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector SPawnLocation = FVector(MyCharacter->GetActorLocation().X, MyCharacter->GetActorLocation().Y, 0);
	FTransform SpawnTransform = FTransform(MyCharacter->GetActorRotation(),SPawnLocation);

	if (ProjectileClass)
	{
		AProPoisonFootprint* SpawnedProjectile = GetWorld()->SpawnActor<AProPoisonFootprint>(ProjectileClass,SpawnTransform);

		int32 FinalDamage = CalculateFinalDamage();
		SpawnedProjectile->SetDamage(FinalDamage);
		SpawnedProjectile->CanCollision = true;
	}
}
