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
	

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if (ProjectileClass && OwnerCharacter && ProjectileSpawnPoint)
	{
		switch (WeaponLevel)
		{
		case 1:
		case 2:
			for (int i = 1; i <= 2; i++)
			{
				AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
				{
					WhirlwindBlade->SpawnLocationByRadians = 180 * i;

					int32 FinalDamage = CalculateFinalDamage();
					WhirlwindBlade->SetDamage(FinalDamage);
					WhirlwindBlade->CanCollision = true;
					WhirlwindBlade->SetReplicates(true); // 복제 설정
					WhirlwindBlade->SetOwner(this);
					WhirlwindBlade->SetLifeSpan(Duration);
				}
			}

			break;
		case 3:
		case 4:
			for (int i = 1; i <= 4; i++)
			{
				AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
				{
					WhirlwindBlade->SpawnLocationByRadians = 90 * i;

					int32 FinalDamage = CalculateFinalDamage();
					WhirlwindBlade->SetDamage(FinalDamage);
					WhirlwindBlade->CanCollision = true;
					WhirlwindBlade->SetReplicates(true); // 복제 설정
					WhirlwindBlade->SetOwner(this);
					WhirlwindBlade->SetLifeSpan(Duration);
				}
			}
			break;
		case 5:
			for (int i = 1; i <= 6; i++)
			{
				AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
				{
					WhirlwindBlade->SpawnLocationByRadians = 60 * i;

					int32 FinalDamage = CalculateFinalDamage();
					WhirlwindBlade->SetDamage(FinalDamage);
					WhirlwindBlade->CanCollision = true;
					WhirlwindBlade->SetReplicates(true); // 복제 설정
					WhirlwindBlade->SetOwner(this);
					WhirlwindBlade->SetLifeSpan(Duration);
				}

			}
			break;
		}
	}

}

void AWeaponWhirlwind::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_Fire, this, &AWeaponWhirlwind::Fire, 15.0f, true, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("Spawned on %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));
	}

	CalculateAttackStatus();

}

void AWeaponWhirlwind::Fire()
{
	Super::Fire();
	UE_LOG(LogTemp, Warning, TEXT("WhirlwindFire"));
	

	if (HasAuthority()) // 서버라면 직접 실행
	{
		UE_LOG(LogTemp, Warning, TEXT("WhirlwindFire On Server"));
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


			UE_LOG(LogTemp, Warning, TEXT("WhirlwindFire"));
			switch (WeaponLevel)
			{
			case 1:
			case 2:
				for (int i = 1; i <= 2; i++)
				{
					AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
					{
						WhirlwindBlade->SpawnLocationByRadians = 180 * i;

						int32 FinalDamage = CalculateFinalDamage();
						WhirlwindBlade->SetDamage(FinalDamage);
						WhirlwindBlade->CanCollision = true;
						WhirlwindBlade->SetLifeSpan(Duration);
						UE_LOG(LogTemp, Warning, TEXT("Duration : %d"),Duration);
					}
				}

				break;
			case 3:
			case 4:
				for (int i = 1; i <= 4; i++)
				{
					AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
					{
						WhirlwindBlade->SpawnLocationByRadians = 90 * i;

						int32 FinalDamage = CalculateFinalDamage();
						WhirlwindBlade->SetDamage(FinalDamage);
						WhirlwindBlade->CanCollision = true;
						WhirlwindBlade->SetLifeSpan(Duration);
					}
				}
				break;
			case 5:
				for (int i = 1; i <= 6; i++)
				{
					AProWhirlwindBlade* WhirlwindBlade = GetWorld()->SpawnActor<AProWhirlwindBlade>(ProjectileClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
					{
						WhirlwindBlade->SpawnLocationByRadians = 60 * i;

						int32 FinalDamage = CalculateFinalDamage();
						WhirlwindBlade->SetDamage(FinalDamage);
						WhirlwindBlade->CanCollision = true;
						WhirlwindBlade->SetLifeSpan(Duration);
					}

				}
				break;
			}
		}
		
	}
	else // 클라이언트라면 서버에 요청
	{
		C2S_SpawnWhirlwindBlade();
		
	}
}
