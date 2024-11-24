// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponWhirlwind.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProWhirlwindBlade.h"

AWeaponWhirlwind::AWeaponWhirlwind()
{
	WeaponType = EWeaponType::WeaponWhirlwind;
	WeaponLevel = 1;
	CoolDown = 7.0f;
	Damage = 55;
	Duration = 7;
	ProjectileCount = 2;


	SetReplicates(true);
	SetReplicateMovement(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

}

void AWeaponWhirlwind::C2S_SpawnWhirlwindBlade_Implementation()
{
	if (!HasAuthority()) // 서버에서만 스폰 실행
	return;
	CalculateAttackStatus();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if (ProjectileClass && OwnerCharacter && ProjectileSpawnPoint)
	{
		for (int i = 1; i <= ProjectileCount; i++)
		{
			AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
			{
				WhirlwindBlade->SpawnLocationByRadians = 360 / ProjectileCount * i ;

				int32 FinalDamage = CalculateFinalDamage();
				WhirlwindBlade->SetDamage(FinalDamage);
				WhirlwindBlade->CanCollision = true;
				WhirlwindBlade->SetReplicates(true); // 복제 설정
				WhirlwindBlade->SetOwner(this);
				WhirlwindBlade->SetLifeSpan(Duration);
			}
		}
	}

}

void AWeaponWhirlwind::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CalculateAttackStatus();
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_Fire, this, &AWeaponWhirlwind::Fire, 15.0f, true, 0.0f);
		
	}

	

}

void AWeaponWhirlwind::Fire()
{
	Super::Fire();

	if (HasAuthority()) // 서버라면 직접 실행
	{
		CalculateAttackStatus();
		UE_LOG(LogTemp, Warning, TEXT("Wind Projectile : %d "),MaxProjectileCnt);
		/*FActorSpawnParameters SpawnPara;*/
		if (!HasAuthority()) // 서버가 아닐 경우 실행하지 않음
		{
			return;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		if (ProjectileClass && OwnerCharacter && ProjectileSpawnPoint)
		{
			for (int i = 1; i <= MaxProjectileCnt; i++)
			{
				AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
				{
					WhirlwindBlade->SpawnLocationByRadians = 360 / MaxProjectileCnt * i;

					int32 FinalDamage = CalculateFinalDamage();
					WhirlwindBlade->SetDamage(FinalDamage);
					WhirlwindBlade->CanCollision = true;
					WhirlwindBlade->SetReplicates(true); // 복제 설정
					WhirlwindBlade->SetOwner(this);
					WhirlwindBlade->SetLifeSpan(Duration);
				}
			}
		}
		
	}
	else // 클라이언트라면 서버에 요청
	{

		C2S_SpawnWhirlwindBlade();
		
	}
}
