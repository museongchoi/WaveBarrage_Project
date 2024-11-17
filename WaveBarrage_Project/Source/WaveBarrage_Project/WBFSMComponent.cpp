// Fill out your copyright notice in the Description page of Project Settings.


#include "WBFSMComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWBFSMComponent::UWBFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MoveDistance = 200.0f;
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

	StateTick(DeltaTime);
}

void UWBFSMComponent::IdleState()
{
	// 방향 조정
	if (IsValid(TargetPlayer))
	{
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetPlayer->GetActorLocation());
		GetOwner()->SetActorRotation(TargetRot);
	}

	ChangeState(EMonsterState::Move);
	
	//if (StateTime > 0.1f)
	//{
	//	ChangeState(EMonsterState::Move);
	//}
}

void UWBFSMComponent::MoveState(float DeltaTime)
{
	// 레이 캐스트 해서 앞에 몬스터로 막혀 있으면 멈추도록 변경
	// 
	//타겟 플레이어 방향으로 이동
	FVector Dist = GetOwner()->GetActorForwardVector() * MoveDistance * DeltaTime;
	GetOwner()->AddActorWorldOffset(Dist, true);

	//아니면서 지속 시간이 끝나면 IdleState
	if (CanAttack)
	{
		//이동 후 CanAttack 이 true 이고 거리가 사거리 안이라면 AttackState
		ChangeState(EMonsterState::Attack);
	}
	else
	{
		if (StateTime > 1.0f)
		{
			ChangeState(EMonsterState::Idle);
		}
	}
}

void UWBFSMComponent::AttackState()
{
	//원거리 , 보스 몬스터 전용
	//투사체 발사 후 일정 시간 지나면 IdleState
	if (StateTime > 1.0f)
	{
		ChangeState(EMonsterState::Idle);
	}
}

void UWBFSMComponent::SkillState()
{
	// 보스 전용
	// 워닝 사인 이후 콜리전 검사해서 범위 내의 플레이어에게 데미지
	// 일정 시간 지나면 IdleState
	if (StateTime > 1.0f)
	{
		ChangeState(EMonsterState::Idle);
	}
}

void UWBFSMComponent::DieState()
{
	// Destroy Actor 진행하는 동안 움직이지 않도록 하기 위함
}


void UWBFSMComponent::StateTick(float DeltaTime)
{
	StateTime += DeltaTime;
	switch (MState)
	{
	case EMonsterState::Idle:
		IdleState();
		break;
	case EMonsterState::Move:
		MoveState(DeltaTime);
		break;
	case EMonsterState::Attack:
		AttackState();
		break;
	case EMonsterState::Skill:
		SkillState();
		break;
	case EMonsterState::Die:
		DieState();
		break;
	default:
		break;
	}
}

void UWBFSMComponent::ChangeState(EMonsterState State)
{
	MState = State;
	StateTime = 0;
}

