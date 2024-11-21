// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponJinx.h"
#include "WBPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBPlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProCuteLauncher.h"

AWeaponJinx::AWeaponJinx()
{
	// 투사체 스폰 포인트 컴포넌트
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	RootComponent = ProjectileSpawnPoint;

	WeaponType = EWeaponType::WeaponJinx;
	WeaponLevel = 1;
	Damage = 20;
	ProjectileCount = 3;
	MaxProjectileCnt = 0;
	CurProjectileCnt = 0;
}

void AWeaponJinx::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerCharacter == nullptr && GetOwner())
	{
		OwnerCharacter = Cast<AWBPlayerBase>(GetOwner());

	}
}

void AWeaponJinx::Fire()
{
	Super::Fire();

	// Get projectile count from player state and update max projectile count
	if (HasAuthority() && OwnerCharacter)
	{

		AWBPlayerState* PlayerState = Cast<AWBPlayerState>(OwnerCharacter->GetPlayerState());
		if (PlayerState)
		{

			MaxProjectileCnt = ProjectileCount + PlayerState->ProjectileCounts;
			
		}
	}

	// Set timer to repeatedly call SpawnProjectile
	UKismetSystemLibrary::K2_SetTimer(this, TEXT("SpawnProjectile"), 0.1f, true);
}

void AWeaponJinx::SpawnProjectile()
{

	if (ProjectileClass && OwnerCharacter && ProjectileSpawnPoint)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Cast<APawn>(OwnerCharacter);
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Get spawn location and rotation from ProjectileSpawnPoint
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		// Spawn the projectile actor
		AProCuteLauncher* SpawnedProjectile = GetWorld()->SpawnActor<AProCuteLauncher>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedProjectile)
		{
			// Increment current projectile count
			CurProjectileCnt++;
			int32 FinalDamage = CalculateFinalDamage();
			SpawnedProjectile->SetDamage(FinalDamage);
			SpawnedProjectile->CanCollision = true;

		}
	}

	// Check if the current projectile count has reached the max
	if (CurProjectileCnt >= MaxProjectileCnt)
	{

		// Clear the timer to stop spawning projectiles
		UKismetSystemLibrary::K2_ClearTimer(this, TEXT("SpawnProjectile"));

		// Reset current projectile count
		CurProjectileCnt = 0;

		if(HasAuthority())
		{
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
			OwnerCharacter->GetMesh()->SetWorldRotation(FRotator(0.0f, OwnerCharacter->GetActorRotation().Yaw - 90.0f, 0.0f));
			OwnerCharacter->ServerSetOrientation(OwnerCharacter->GetActorRotation().Yaw, true);
		}
		

		// 서버에서 플레이어의 원래 방향으로 회전 설정
		if (AWBPlayerBase* PlayerBase = Cast<AWBPlayerBase>(OwnerCharacter))
		{
			PlayerBase->ServerSetOrientation(OwnerCharacter->GetActorRotation().Yaw, true);
		}

	}
}
