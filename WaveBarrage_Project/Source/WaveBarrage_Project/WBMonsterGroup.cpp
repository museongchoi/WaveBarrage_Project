// Fill out your copyright notice in the Description page of Project Settings.


#include "WBMonsterGroup.h"
#include "WBMonsterBase.h"
#include "WBGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWBMonsterGroup::AWBMonsterGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void AWBMonsterGroup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWBMonsterGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnEnd && Monsters.Num() > 0)
	{
		if (IsValid(Monsters[0]))
		{
			SetActorLocation(Monsters[0]->GetActorLocation());
		}
	}
}

void AWBMonsterGroup::SpawnMonster()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPlayer->GetActorLocation());
	SetActorRotation(TargetRotation);

	FActorSpawnParameters SpawnPara;
	SpawnPara.Owner = this;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (MonsterClass && HasAuthority())
	{
		for (int i = 0; i < SpawnCount; i++)
		{
			int Row = FMath::CeilToInt(i / 8.0f);
			float Angle = i * (2 * PI / 8);
			float X = 100 * Row * FMath::Cos(Angle);
			float Y = 100 * Row * FMath::Sin(Angle);
			float Z = 0;

			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, GetActorLocation() + FVector(X, Y, 0), GetActorRotation(), SpawnPara);
			if (IsValid(Spawned))
			{
				Monsters.Emplace(Spawned);
				Spawned->SetLifeSpan(30.0f);
			}
		}
		SpawnEnd = true;
	}
}

void AWBMonsterGroup::SpawnRandomPositionMonster()
{
	FActorSpawnParameters SpawnPara;
	SpawnPara.Owner = this;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (MonsterClass && HasAuthority())
	{
		for (int i = 0; i < SpawnCount; i++)
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, GetActorLocation() + FVector(FMath::RandRange(-1000, 1000), FMath::RandRange(-1000, 1000), 0), GetActorRotation(), SpawnPara);
			if (IsValid(Spawned))
			{
				Spawned->SetTargetPlayer(TargetPlayer);
				Monsters.Emplace(Spawned);
			}
		}
		SpawnEnd = true;
	}
}

void AWBMonsterGroup::SpawnCirclePositionMonster()
{
	FActorSpawnParameters SpawnPara;
	SpawnPara.Owner = this;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (MonsterClass && HasAuthority())
	{
		for (int i = 0; i < SpawnCount; i++)
		{
			float Angle = i * (2 * PI / SpawnCount);
			float X = 1500 * FMath::Cos(Angle);
			float Y = 1500 * FMath::Sin(Angle);
			float Z = 0;

			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, GetActorLocation() + FVector(X,Y,0), GetActorRotation(), SpawnPara);
			if (IsValid(Spawned))
			{
				Spawned->SetTargetPlayer(TargetPlayer);
				Monsters.Emplace(Spawned);
				Spawned->SetLifeSpan(45.0f);
			}
		}
		SpawnEnd = true;
	}
}

void AWBMonsterGroup::RemoveMonster(AWBMonsterBase* Monster)
{
	Monsters.Remove(Monster);
	int AliveCount = Monsters.Num();
	for (AWBMonsterBase* WBMonster : Monsters)
	{
		if (!IsValid(WBMonster))
		{
			AliveCount--;
		}
	}
	if (AliveCount <= 0)
	{
		AWBGameMode* GM = Cast<AWBGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(GM))
		{
			GM->MonsterGroups.Remove(this);
		}
		Destroy();
	}
}

void AWBMonsterGroup::UpdateTargetPlayer()
{
	if (HasAuthority())
	{
		for (AWBMonsterBase* Monster : Monsters)
		{
			if (IsValid(Monster))
			{
				Monster->SetTargetPlayer(TargetPlayer);
			}
		}
	}
}

