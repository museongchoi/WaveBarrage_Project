// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "WBSapwnDataTable.h"
#include "WBWeaponDataTable.h"
#include "WBPlayerController.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AWBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AWBPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowCardSelectionWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> widgetClass;

	UUserWidget* CardSelectionWidgetInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* WeaponDataTable;

	UFUNCTION(BlueprintCallable)
	void CardClicked();

	UFUNCTION(Server,Reliable)
	void C2S_SetPlayerReady();
	void C2S_SetPlayerReady_Implementation();

	UFUNCTION( Category = "Card Selection", Client, Reliable)
	void ApplyCardEffect(AWBPlayerController* PlayerController, int32 CardIndex);
	void ApplyCardEffect_Implementation(AWBPlayerController* PlayerController, int32 CardIndex);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void C2S_ApplyCardEffect(AWBPlayerController* PlayerController, int32 CardIndex);
	void C2S_ApplyCardEffect_Implementation(AWBPlayerController* PlayerController, int32 CardIndex);


	//UFUNCTION(Server, Reliable)
	//void C2S_SpawnWeapon(int32 CardIndex, AWBPlayerBase* MyPlayer);
	//void C2S_SpawnWeapon_Implementation(int32 CardIndex, AWBPlayerBase* MyPlayer);


};
