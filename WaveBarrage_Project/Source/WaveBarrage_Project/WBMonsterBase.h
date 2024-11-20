// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBMonsterBase.generated.h"

class USphereComponent;
class UWBFSMComponent;



UCLASS()
class WAVEBARRAGE_PROJECT_API AWBMonsterBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWBMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	void Attack();
	void Skill();

	void SetTargetPlayer(AActor* Target);

	//void Destroy();

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWBFSMComponent> FSMComp;

	UPROPERTY(EditAnywhere)
	int HP;

	UPROPERTY(EditAnywhere)
	float AttackPoint;

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWBItemBase> DropItem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWBMonsterProjectile> MonsterProjectile;

};
