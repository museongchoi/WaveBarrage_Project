// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameMode.h"
#include "WBPlayerController.h"
#include "WBMonsterBase.h"
#include "WBPlayerBase.h"
#include "WBMonsterGroup.h"

AWBGameMode::AWBGameMode()
{

}

void AWBGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<FSpawnData*> arr;
	SpawnDataTable->GetAllRows<FSpawnData>(TEXT("GetAllRows"), arr);

	for (FSpawnData* Data : arr)
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("SpawnMonster"), Data->SpawnType, Data->MonsterClass, Data->SpawnCount, Data->x, Data->y);

		FTimerHandle Handle;
		FTimerManagerTimerParameters Para;
		GetWorld()->GetTimerManager().SetTimer(Handle, TimerDel, Data->SpawnTime, false);
	}


}

void AWBGameMode::AddExp(int Value)
{
	Exp += Value;
}

void AWBGameMode::LevelUp()
{

	for (AActor* PlayerActor : Players)
	{
		AWBPlayerBase* Player = Cast<AWBPlayerBase>(PlayerActor);
		if (Player)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
			if (PlayerController)
			{
				AWBPlayerController* WBPlayerController = Cast<AWBPlayerController>(PlayerController);
				if(WBPlayerController)
				{
					WBPlayerController->ShowCardSelectionWidget();
				}
			}
		}
	}

}


void AWBGameMode::ApplyCardEffect(AWBPlayerController* PlayerController, int32 CardIndex)
{
	AWBPlayerState* PlayerState = PlayerController->GetPlayerState<AWBPlayerState>();
	if (!PlayerState)
	{
		return;
	}

	if (CardIndex >= 0 && CardIndex <= 4)
	{
		FName RowName = FName(*FString::FromInt(CardIndex + 1));
		//FWeaponData* Wea
	}
}

void AWBGameMode::SetTargetPlayer()
{

}

void AWBGameMode::SpawnMonster(ESpawnType SpawnType, TSubclassOf<AWBMonsterBase> MonsterClass, int SpawnCount, float x, float y)
{
	FActorSpawnParameters SpawnPara;
	SpawnPara.Owner = this;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	switch (SpawnType)
	{
	case ESpawnType::Normal:
		{
			for (AWBPlayerBase* Player : Players)
			{
				if (IsValid(Player))
				{
					for (USceneComponent* Comp : Player->MonsterSpawnPositions)
					{
						AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Comp->GetComponentLocation(), FRotator::ZeroRotator, SpawnPara);
						if (Spawned)
						{
							Spawned->TargetPlayer = Player;
							Spawned->MonsterClass = MonsterClass;
							Spawned->SpawnCount = SpawnCount;
							MonsterGroups.Emplace(Spawned);
							Spawned->SpawnMonster();
						}
					}
				}
			}
		}
		break;
	case ESpawnType::Minion:
		{
			for (AWBPlayerBase* Player : Players)
			{
				if (IsValid(Player))
				{

					AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Player->GetActorLocation() + FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
					if (Spawned)
					{
						Spawned->TargetPlayer = Player;
						Spawned->MonsterClass = MonsterClass;
						Spawned->SpawnCount = SpawnCount;
						MonsterGroups.Emplace(Spawned);
						Spawned->SpawnMonster();
					}
				}
			}
		}
		break;
	case ESpawnType::Circle:
		{
			for (AWBPlayerBase* Player : Players)
			{
				if (IsValid(Player))
				{

					AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Player->GetActorLocation(), FRotator::ZeroRotator, SpawnPara);
					if (Spawned)
					{
						Spawned->TargetPlayer = Player;
						Spawned->MonsterClass = MonsterClass;
						Spawned->SpawnCount = SpawnCount;
						MonsterGroups.Emplace(Spawned);
						Spawned->SpawnMonster();
					}
				}
			}
		}
		break;
	case ESpawnType::Elite:
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
			Spawned->SetTargetPlayer(GetNearPlayer(Spawned));
			
		}
		break;
	case ESpawnType::Boss:
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
			Spawned->SetTargetPlayer(GetNearPlayer(Spawned));
		}
		break;
	default:
		break;
	}



}

AActor* AWBGameMode::GetNearPlayer(AActor* Monster)
{
	float Near = 9999.9999f;
	AActor* NearPlayer = nullptr;
	for (AActor* Player : Players)
	{
		float Dist = FVector::Dist(Monster->GetActorLocation(), Player->GetActorLocation());
		if (Near > Dist)
		{
			Near = Dist;
			NearPlayer = Player;
		}
	}

	return NearPlayer;
}
