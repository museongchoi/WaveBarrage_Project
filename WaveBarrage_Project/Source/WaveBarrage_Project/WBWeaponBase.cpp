// Fill out your copyright notice in the Description page of Project Settings.


#include "WBWeaponBase.h"
#include "Components/SceneComponent.h"
#include "WBPlayerBase.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBProjectileBase.h"
#include "Net/UnrealNetwork.h"
#include "WBPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "WBPlayerController.h"

// Sets default values
AWBWeaponBase::AWBWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AWBWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AWBWeaponBase::Fire()
//{
//	//if (ProjectileClass && OwnerCharacter)
//	//{
//	//	FActorSpawnParameters SpawnParams;
//	//	SpawnParams.Owner = this;
//
//	//	FVector SpawnLocation = GetActorLocation();
//	//	FRotator SpawnRotation = GetActorRotation();
//
//	//	// 투사체 생성
//	//	AWBProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AWBProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
//	//	if (SpawnedProjectile)
//	//	{
//	//		// 데미지 설정
//	//		int32 FinalDamage = CalculateFinalDamage();
//	//		SpawnedProjectile->SetDamage(FinalDamage);
//	//	}
//	//}
//}

void AWBWeaponBase::Fire_Implementation()
{
	// 투사체 스폰 로직이 여기 들어갑니다
	// 예: GetWorld()->SpawnActor<AWBProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
}

void AWBWeaponBase::SpawnProjectile()
{

}

EWeaponType AWBWeaponBase::GetWeaponType() const
{
	return WeaponType;
}

int32 AWBWeaponBase::CalculateFinalDamage()
{
	// 무기의 기본 데미지를 사용
	int32 WeaponBaseDamage = Damage;

	// OwnerCharacter의 PlayerState에서 패시브 데미지 증가율 가져오기
	if (OwnerCharacter)
	{
		AWBPlayerState* PlayerState = OwnerCharacter->GetPlayerState<AWBPlayerState>();
		if (PlayerState)
		{
			int32 DamageMultiplier = Damage + ((Damage * PlayerState->Damage)/100);
			return DamageMultiplier;
		}
	}

	return WeaponBaseDamage;
}

void AWBWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWBWeaponBase, WeaponLevel);
	DOREPLIFETIME(AWBWeaponBase, WeaponType);
	DOREPLIFETIME(AWBWeaponBase, Damage);
	DOREPLIFETIME(AWBWeaponBase, SkillAcceleration);
	DOREPLIFETIME(AWBWeaponBase, ProjectileCount);
	DOREPLIFETIME(AWBWeaponBase, CurProjectileCnt);
	DOREPLIFETIME(AWBWeaponBase, MaxProjectileCnt);
	DOREPLIFETIME(AWBWeaponBase, CoolDown);
	DOREPLIFETIME(AWBWeaponBase, CriticalChance);
	DOREPLIFETIME(AWBWeaponBase, Duration);
}

void AWBWeaponBase::CalculateAttackStatus()
{
	AWBPlayerController* MyPlayerController = Cast<AWBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (MyPlayerController->IsLocalPlayerController())
	{
		AWBPlayerState* MyPlayerState = MyPlayerController->GetPlayerState<AWBPlayerState>();
		if (MyPlayerState)
		{	
			CriticalChance = MyPlayerState->CriticalHitChance * 0.01f;
			MaxProjectileCnt = ProjectileCount + MyPlayerState->ProjectileCounts;
			if (SkillAcceleration > 0)
			{
				CoolDown = CoolDown * ((1 - MyPlayerState->SkillAcceleration) / (MyPlayerState->SkillAcceleration + 100));
			}

		}
	}
}

int32 AWBWeaponBase::CanCritialAttack(int32 WeaponBaseDamage)
{
	WeaponBaseDamage = Damage;

	bool bCanCritical = false;

	float Random = FMath::FRand(); // 0~1

	if (Random <= CriticalChance)
	{
		bCanCritical = true;
	}

	if (!bCanCritical)
	{
		return WeaponBaseDamage;
	}

	return WeaponBaseDamage * 1.5f;
}


