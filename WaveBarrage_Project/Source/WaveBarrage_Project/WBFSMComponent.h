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
	Skill,
	Die
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
	void MoveState();
	void AttackState();
	void SkillState();
	void DieState();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeState();

public:
	EMonsterState MState;
	bool CanAttack = false;
	bool CanSkill = false;
};
