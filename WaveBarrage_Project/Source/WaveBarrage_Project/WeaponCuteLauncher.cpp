// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCuteLauncher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WBMonsterBase.h"
#include "Engine/World.h"
#include "WBPlayerState.h"

// MaxProjectileCnt = ProjectileCount + PlayerState->ProjectileCounts
// ProjectileCount : 1/1/2/2/3
// Damage : 45/50/55/60/65
AWeaponCuteLauncher::AWeaponCuteLauncher()
{
	ClosestDistance = FLT_MAX;
	CoolDown = 1.0f;
	Damage = 45;
	ProjectileCount = 1;

	if (OwnerCharacter)
	{
		AWBPlayerState* PlayerState = Cast<AWBPlayerState>(OwnerCharacter->GetPlayerState());
		if (PlayerState)
		{
			MaxProjectileCnt = ProjectileCount + PlayerState->ProjectileCounts;
		}
	}
}

void AWeaponCuteLauncher::BeginPlay()
{
	//Super::BeginPlay();

	if (!GetWorld()->GetTimerManager().IsTimerActive(FTimerHandle_AutomaticAiming))
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_AutomaticAiming, this, &AWeaponCuteLauncher::CuteLauncherAutomaticAiming, 0.01f, true);
	}
	if(!GetWorld()->GetTimerManager().IsTimerActive(FTimerHandle_AttackFire))
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_AttackFire, this, &AWeaponCuteLauncher::Fire, CoolDown, true);
	}
}

void AWeaponCuteLauncher::Fire()
{
	if (ProjectileClass && ProjectileSpawnPoint)
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
			UE_LOG(LogTemp, Error, TEXT("%d CuteLauncher"), CurProjectileCnt);
		}
	}
	if (CurProjectileCnt >= MaxProjectileCnt)
	{
		return;
	}
}

void AWeaponCuteLauncher::SpawnProjectile()
{

}

void AWeaponCuteLauncher::CuteLauncherAutomaticAiming()
{
	ClosestDistance = FLT_MAX;
	ClosestEnemy = nullptr;

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 1000.0f;
	float Radius = 1200.0f;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		OutHit,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius),
		CollisionParams
	);

	// 디버그용 구체 표시 (한 프레임 동안 표시)
	DrawDebugSphere(GetWorld(), Start, Radius, 12, FColor::Red, false, -1.f, 0, 1.0f);

	if (bHit && OutHit.GetActor())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), OutHit.GetActor()->GetClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			if (Actor->IsA(AWBMonsterBase::StaticClass()))
			{
				UE_LOG(LogTemp, Error, TEXT("Actor : %s"), *Actor->GetName());

				float Distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestEnemy = Actor;
				}
			}
		}
	}
}
