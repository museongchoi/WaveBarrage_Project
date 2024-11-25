// Fill out your copyright notice in the Description page of Project Settings.


#include "WBGameMode.h"
#include "WBGameState.h"
#include "WBPlayerController.h"
#include "WBPlayerState.h"
#include "WBMonsterBase.h"
#include "WBPlayerBase.h"
#include "WBMonsterGroup.h"
#include "Kismet/GameplayStatics.h"
//#include "WBULobbyWidget.h"


AWBGameMode::AWBGameMode()
{
	Level = 1;
	MaxExp = 5;
}

void AWBGameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle THandle;
	GetWorld()->GetTimerManager().SetTimer(THandle, this, &AWBGameMode::UpdateTargetPlayer, 2.0f, true);

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
	UpdateGameState();

}

void AWBGameMode::AddExp(int Value)
{
	Exp += Value;
	UE_LOG(LogTemp, Warning, TEXT("EXP : %i"), Exp);
	if (MaxExp <= Exp)
	{
		Exp = 0;
		LevelUp();
		Level += 1;
		if (Level < 5)
		{
			MaxExp += 5;
		}
		else if (Level < 10)
		{
			MaxExp += 1;
		}
		else if(Level < 15)
		{
			MaxExp += 5;
		}
		else if (Level < 20)
		{
			MaxExp += 10;
		}
		else if (Level < 25)
		{
			MaxExp += 1;
		}
		else
		{
			MaxExp += 5;
		}
	}
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
	UpdateGameState();
}

void AWBGameMode::UpdateTargetPlayer()
{
	for (AWBMonsterGroup* MG : MonsterGroups)
	{
		AWBGameState* GS = Cast<AWBGameState>(GameState);
		if (IsValid(GS) && IsValid(MG))
		{
			if (!MG->IsNotUpdate)
			{
				AActor* NearPlayer = GetNearPlayer(MG);
				MG->TargetPlayer = NearPlayer;
				MG->UpdateTargetPlayer();
				GS->S2C_MGSetTargetPlayer(MG, NearPlayer);
			}
		}
	}
}

void AWBGameMode::UpdateGameState()
{
	AWBGameState* MyGameState = GetGameState<AWBGameState>();
	if (MyGameState)
	{
		MyGameState->Level = Level;
		MyGameState->Exp = Exp;
		MyGameState->MaxExp = MaxExp;
	}

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
			if (IsValid(Players[0]))
			{
				for (USceneComponent* Comp : Players[0]->MonsterSpawnPositions)
				{
					AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Comp->GetComponentLocation(), FRotator::ZeroRotator, SpawnPara);
					if (Spawned)
					{
						AWBGameState* GS = Cast<AWBGameState>(GameState);
						if (IsValid(GS))
						{
							GS->S2C_MGSetTargetPlayer(Spawned, Players[0]);
						}
						Spawned->TargetPlayer = Players[0];
						Spawned->MonsterClass = MonsterClass;
						Spawned->SpawnCount = SpawnCount;
						MonsterGroups.Emplace(Spawned);
						Spawned->SpawnRandomPositionMonster();
					}
				}
			}
		}
		break;
	case ESpawnType::Minion:
		{
			if (IsValid(Players[0]))
			{

				AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Players[0]->GetActorLocation() + FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
				if (Spawned)
				{
					AWBGameState* GS = Cast<AWBGameState>(GameState);
					if (IsValid(GS))
					{
						GS->S2C_MGSetTargetPlayer(Spawned, Players[0]);
					}
					Spawned->TargetPlayer = Players[0];
					Spawned->MonsterClass = MonsterClass;
					Spawned->SpawnCount = SpawnCount;
					Spawned->IsNotUpdate = true;
					MonsterGroups.Emplace(Spawned);
					Spawned->SpawnMonster();
				}
			}
		}
		break;
	case ESpawnType::Circle:
		{
			if (IsValid(Players[0]))
			{

				AWBMonsterGroup* Spawned = GetWorld()->SpawnActor<AWBMonsterGroup>(AWBMonsterGroup::StaticClass(), Players[0]->GetActorLocation(), FRotator::ZeroRotator, SpawnPara);
				if (Spawned)
				{
					AWBGameState* GS = Cast<AWBGameState>(GameState);
					if (IsValid(GS))
					{
						GS->S2C_MGSetTargetPlayer(Spawned, Players[0]);
					}
					Spawned->TargetPlayer = Players[0];
					Spawned->MonsterClass = MonsterClass;
					Spawned->SpawnCount = SpawnCount;
					MonsterGroups.Emplace(Spawned);
					Spawned->SpawnCirclePositionMonster();
				}
			}
		}
		break;
	case ESpawnType::Elite:
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, FVector(x, y, 50), FRotator::ZeroRotator, SpawnPara);
			AWBGameState* GS = Cast<AWBGameState>(GameState);
			if (IsValid(GS))
			{
				GS->S2C_MBSetTargetPlayer(Spawned, Players[0]);
			}
			Spawned->SetTargetPlayer(GetNearPlayer(Players[0]));
			
		}
		break;
	case ESpawnType::Boss:
		{
			AWBMonsterBase* Spawned = GetWorld()->SpawnActor<AWBMonsterBase>(MonsterClass, FVector(x, y, 0), FRotator::ZeroRotator, SpawnPara);
			Spawned->SetTargetPlayer(Players[0]);
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
