// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerController.h"
#include "WBGameState.h"
#include "WBPlayerState.h"
#include "WBMonsterBase.h"
#include "WBPlayerBase.h"
#include "WBMonsterGroup.h"
#include "Kismet/GameplayStatics.h"
#include "WBGameMode.h"
//void AWBPlayerController::ShowCardSelectionWidget()
//{
	//if (widgetClass)
	//{
 //       if (!CardSelectionWidgetInstance)
 //       {
 //           CardSelectionWidgetInstance = CreateWidget<UUserWidget>(this, widgetClass);
 //           if (CardSelectionWidgetInstance)
 //           {
 //               CardSelectionWidgetInstance->AddToViewport();
 //           }
 //       }
	//}
//}

AWBPlayerController::AWBPlayerController()
{
	WeaponDataTable = CreateDefaultSubobject<UDataTable>("WeaponDataTable");
}

void AWBPlayerController::BeginPlay()
{
	Super::BeginPlay();


}

void AWBPlayerController::CardClicked()
{
	C2S_SetPlayerReady();
}

void AWBPlayerController::C2S_SetPlayerReady_Implementation()
{
	AWBPlayerState* MyPlayerState = GetPlayerState<AWBPlayerState>();
	if (MyPlayerState)
	{
		MyPlayerState->SetPlayerState(EPlayerState::Ready);
	}

	AWBGameState* MyGameState = GetWorld()->GetGameState<AWBGameState>();
	if (MyGameState)
	{
		MyGameState->CheckAllPlayersReady();
	}
}

void AWBPlayerController::ApplyCardEffect_Implementation(AWBPlayerController* PlayerController, int32 CardIndex)
{
	if (PlayerController->IsLocalPlayerController())
	{
		AWBPlayerBase* MyPlayer = Cast<AWBPlayerBase>(PlayerController->GetCharacter());
		if (MyPlayer)
		{
			AWBPlayerState* MyPlayerState = MyPlayer->GetPlayerState<AWBPlayerState>();
			if (!MyPlayerState)
			{
				return;
			}

			// 무기 카드 처리 (0-4)
			if (CardIndex >= 0 && CardIndex <= 4)
			{

				FString WeaponTypeString;
				switch (static_cast<EWeaponType>(CardIndex))
				{

				case EWeaponType::WeaponJinx:
					WeaponTypeString = "Jinx";
					break;
				case EWeaponType::WeaponWhirlwind:
					WeaponTypeString = "Whirlwind";
					break;
				case EWeaponType::WeaponPoisonFootprint:
					WeaponTypeString = "PoisonFootprint";
					break;
				case EWeaponType::WeaponBoomerang:
					WeaponTypeString = "Boomerang";
					break;
				case EWeaponType::WeaponCuteLauncher:
					WeaponTypeString = "CuteLauncher";
					break;
				default:
					return; // 유효하지 않은 무기 타입인 경우 리턴
				}
				// 무기 레벨 가져오기
				int16* CurrentLevelPtr = MyPlayerState->ItemLevel.Find(WeaponTypeString);
				int MyWeaponLevel = (CurrentLevelPtr) ? FMath::Min((*CurrentLevelPtr) + 1, 5) : 1;

				// 데이터 테이블에서 해당 무기와 레벨의 능력치를 가져오기
				FName RowName = FName(*FString::Printf(TEXT("%s_%d"), *WeaponTypeString, MyWeaponLevel));
				FWeaponData* WeaponData = WeaponDataTable->FindRow<FWeaponData>(RowName, TEXT(""));
				if (WeaponData)
				{
					 //로그 : 무기 업데이트 전 데미지 출력
					/*for (AWBWeaponBase* Weapon : MyPlayer->EquippedWeapons)
					{
						if (Weapon && Weapon->GetWeaponType() == static_cast<EWeaponType>(CardIndex))
						{
							UE_LOG(LogTemp, Warning, TEXT(" CurWeapon: %s"), *WeaponTypeString);
						}
					}*/

					bool bWeaponExists = false;


					// 무기 업데이트
					for (AWBWeaponBase* Weapon : MyPlayer->EquippedWeapons)
					{
						if (Weapon && Weapon->GetWeaponType() == static_cast<EWeaponType>(CardIndex))
						{
							Weapon->WeaponLevel = WeaponData->WeaponLevel;
							Weapon->Damage = WeaponData->Damage;
							Weapon->SkillAcceleration = WeaponData->SkillAcceleration;
							Weapon->CoolDown = WeaponData->CoolDown;
							Weapon->CriticalChance = WeaponData->CriticalChance;
							Weapon->ProjectileCount = WeaponData->ProjectileCount;


							// 무기 업데이트 후 데미지 출력
							UE_LOG(LogTemp, Warning, TEXT("After Update - Weapon: %s, Level: %d"), *WeaponTypeString, Weapon->WeaponLevel);

							bWeaponExists = true;

							break;
						}
					}
					UE_LOG(LogTemp, Warning, TEXT("bWeaponExists value: %s"), bWeaponExists ? TEXT("true") : TEXT("false"));

					//if (!bWeaponExists)
					//{
					//	UE_LOG(LogTemp, Warning, TEXT("ReqWeaponCreate"));
					//	C2S_SpawnWeapon(CardIndex, MyPlayer);
					//}
					if (!bWeaponExists)
					{
						//// 새로운 무기 생성
						//FActorSpawnParameters SpawnParams;
						//SpawnParams.Owner = MyPlayer;
						//AWBWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(MyPlayer->WeaponAttachBoxes[CardIndex], MyPlayer->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
						//if (NewWeapon)
						//{
						//	NewWeapon->AttachToComponent(MyPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
						//	NewWeapon->OwnerCharacter = MyPlayer;
						//	MyPlayer->EquippedWeapons.Add(NewWeapon);
						//}
						
						C2S_SpawnWeapon(CardIndex, MyPlayer);
						
					}


					// 무기 레벨 업데이트
					MyPlayerState->ItemLevel.Add(WeaponTypeString, MyWeaponLevel);
				}

			}
			// 패시브 카드 처리 (5-9)
			else if (CardIndex >= 5 && CardIndex <= 9)
			{

				// 패시브 처리 로직은 이전과 동일합니다.
				FString PassiveKey = FString::FromInt(CardIndex);
				int16* CurrentLevelPtr = MyPlayerState->ItemLevel.Find(PassiveKey);
				int PassiveLevel = (CurrentLevelPtr) ? (*CurrentLevelPtr) + 1 : 1;

				switch (CardIndex)
				{
				case 5:
					MyPlayerState->Damage += 10; // 레벨당 +10%, 최대 50%
					break;
				case 6:
					MyPlayerState->CriticalHitChance += 8; // 레벨당 치명타 확률 8% 증가, 최대 +40%
					break;
				case 7:
					MyPlayerState->SkillAcceleration += 10; // 레벨당 스킬 가속 10 증가, 최대 +50
					break;
				case 8:
					MyPlayerState->ProjectileCounts+= 1; // 레벨당 이동 속도 8% 증가, 최대 +40%
					break;
				case 9:
					MyPlayerState->MaxHealth += 4; //레벨당 초당 체력 재생 4 증가, 최대 +20
					break;
				default:
					break;
				}

				// 패시브 레벨 업데이트
				MyPlayerState->ItemLevel.Add(FString::FromInt(CardIndex), 1); // 패시브 레벨 추가 (기본 레벨 1)
			}
		}
	}
}

void AWBPlayerController::C2S_ApplyCardEffect_Implementation(AWBPlayerController* PlayerController, int32 CardIndex)
{
	if (HasAuthority())
	{
		ApplyCardEffect(PlayerController, CardIndex);
	}
}

void AWBPlayerController::C2S_SpawnWeapon_Implementation(int32 CardIndex, AWBPlayerBase* MyPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("WeaponCreateStart"));
	if (HasAuthority()) // 서버에서만 실행
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = MyPlayer;

		AWBWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(
			MyPlayer->WeaponAttachBoxes[CardIndex],
			MyPlayer->GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams);

		if (NewWeapon)
		{
			NewWeapon->AttachToComponent(MyPlayer->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
			NewWeapon->OwnerCharacter = MyPlayer;
			MyPlayer->EquippedWeapons.Add(NewWeapon);
			UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter : %s"), *NewWeapon->OwnerCharacter->GetName());
		}
	}

}