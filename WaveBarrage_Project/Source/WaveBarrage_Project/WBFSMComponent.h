// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WBFSMComponent.generated.h"


enum class EMonsterState : uint8
{
	Idle = 0,
	Move,
	Attack,
	Skill
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WAVEBARRAGE_PROJECT_API UWBFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWBFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void IdleState();
	void MoveState(float DeltaTime);
	void AttackState();
	void SkillState();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StateTick(float DeltaTime);
	void ChangeState(EMonsterState State);

public:
	EMonsterState MState;
	UPROPERTY(EditAnywhere)
	bool CanAttack = false;
	UPROPERTY(EditAnywhere)
	bool CanSkill = false;
	UPROPERTY(EditAnywhere)
	bool CanMove = true;

	AActor* TargetPlayer;

	float StateTime;
	UPROPERTY(EditAnywhere)
	float MoveDistance;
	float AttackTime;
	float SkillTime;
	bool bIsAttackDelay;
	bool bIsSkillDelay;
};
