// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Exp1 = 0 UMETA(DisplayName = "Exp1"),
	Exp2 = 1 UMETA(DisplayName = "Exp2"),
	Exp3 = 2 UMETA(DisplayName = "Exp3"),
	Card = 3 UMETA(DisplayName = "Card"),
	Heal = 4 UMETA(DisplayName = "Heal")
};


UCLASS()
class WAVEBARRAGE_PROJECT_API AWBItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWBItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> Root;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

public:
	UPROPERTY(EditAnywhere)
	EItemType ItemType;

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
