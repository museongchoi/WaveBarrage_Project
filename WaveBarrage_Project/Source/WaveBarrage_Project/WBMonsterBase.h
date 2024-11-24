// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBMonsterBase.generated.h"

class USphereComponent;
class UWBFSMComponent;
class UFloatingPawnMovement;



UCLASS()
class WAVEBARRAGE_PROJECT_API AWBMonsterBase : public APawn
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
	void MoveFroward();

	void SetTargetPlayer(AActor* Target);
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//void Destroy();

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UWBFSMComponent> FSMComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UFloatingPawnMovement> FloatMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
	int HP;

	UPROPERTY(EditAnywhere)
	float AttackPoint;

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWBItemBase> DropItem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWBMonsterProjectile> MonsterProjectile;

	UPROPERTY(BlueprintReadWrite)
	bool IsAttack = false;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
