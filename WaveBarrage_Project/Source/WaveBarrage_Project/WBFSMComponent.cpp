// Fill out your copyright notice in the Description page of Project Settings.


#include "WBFSMComponent.h"

// Sets default values for this component's properties
UWBFSMComponent::UWBFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWBFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	MState = EMonsterState::Idle;
	
}

void UWBFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (MState)
	{
	case EMonsterState::Idle:
		IdleState(DeltaTime);
		break;
	case EMonsterState::Move:
		MoveState(DeltaTime);
		break;
	case EMonsterState::Attack:
		AttackState(DeltaTime);
		break;
	case EMonsterState::Skill:
		SkillState(DeltaTime);
		break;
	case EMonsterState::Die:
		DieState(DeltaTime);
		break;
	default:
		break;
	}
}

void UWBFSMComponent::IdleState(float DeltaTime)
{
	// 잠시 대기
	StateTime += DeltaTime;

	if (StateTime > 0.25f)
	{
		MState = EMonsterState::Move;
		StateTime = 0;
	}
}

void UWBFSMComponent::MoveState(float DeltaTime)
{
	StateTime += DeltaTime;
	//타겟 플레이어 방향으로 이동
	
	//아니면서 지속 시간이 끝나면 IdleState
	if (CanAttack)
	{
		//이동 후 CanAttack 이 true 이고 거리가 사거리 안이라면 AttackState

	}
	else
	{
		if (StateTime > 1.0f)
		{
			MState = EMonsterState::Idle;
			StateTime = 0;
		}
	}
}

void UWBFSMComponent::AttackState(float DeltaTime)
{
	//원거리 , 보스 몬스터 전용
	//투사체 발사 후 일정 시간 지나면 IdleState
	if (StateTime > 1.0f)
	{
		MState = EMonsterState::Idle;
		StateTime = 0;
	}
}

void UWBFSMComponent::SkillState(float DeltaTime)
{
	// 보스 전용
	// 워닝 사인 이후 콜리전 검사해서 범위 내의 플레이어에게 데미지
	// 일정 시간 지나면 IdleState
	if (StateTime > 1.0f)
	{
		MState = EMonsterState::Idle;
		StateTime = 0;
	}
}

void UWBFSMComponent::DieState(float DeltaTime)
{
	// Destroy Actor 진행하는 동안 움직이지 않도록 하기 위함
}


// Called every frame


void UWBFSMComponent::ChangeState()
{
}

