// Fill out your copyright notice in the Description page of Project Settings.


#include "WBFSMComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WBMonsterBase.h"
#include "WBMonsterProjectile.h"

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

	Base = Cast<AWBMonsterBase>(GetOwner());
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
		ChangeState(EMonsterState::Move);
	}
	else if (bIsMoveStraight)
	{
		ChangeState(EMonsterState::Move);
	}
}

void UWBFSMComponent::MoveState(float DeltaTime)
{
	//타겟 플레이어 방향으로 이동

	
	if (IsValid(Base))
	{
		Base->MoveFroward();
	}


	//아니면서 지속 시간이 끝나면 IdleState
	if (CanAttack && !bIsAttackDelay)
	{
		//이동 후 CanAttack 이 true 이고 거리가 사거리 안이라면 AttackState
		ChangeState(EMonsterState::Attack);
	}
	else if (CanSkill && !bIsSkillDelay)
	{
		ChangeState(EMonsterState::Skill);
	}
	else
	{
		if (!bIsMoveStraight && StateTime > 1.0f)
		{
			ChangeState(EMonsterState::Idle);
		}
	}
}

void UWBFSMComponent::AttackState()
{
	//원거리 전용
	//투사체 발사 후 일정 시간 지나면 IdleState
	if (!bIsAttackDelay)
	{
		bIsAttackDelay = true;
		Cast<AWBMonsterBase>(GetOwner())->Attack();
	}

	if (StateTime > 0.5f)
	{
		ChangeState(EMonsterState::Idle);
	}
}

void UWBFSMComponent::SkillState()
{
	// 엘리트 전용
	// 워닝 사인 이후 콜리전 검사해서 범위 내의 플레이어에게 데미지
	// 일정 시간 지나면 IdleState
	if (!bIsSkillDelay)
	{
		bIsSkillDelay = true;
		AWBMonsterBase* Monster = Cast<AWBMonsterBase>(GetOwner());
		Monster->Skill();
		Monster->IsAttack = true;
	}

	if (StateTime > 1.0f)
	{
		Cast<AWBMonsterBase>(GetOwner())->IsAttack = false;
		ChangeState(EMonsterState::Idle);
	}
}


void UWBFSMComponent::StateTick(float DeltaTime)
{
	StateTime += DeltaTime;
	if (bIsAttackDelay)
	{
		AttackTime += DeltaTime;
		if (AttackTime > 5.0f)
		{
			bIsAttackDelay = false;
			AttackTime = 0;
		}
	}
	if (bIsSkillDelay)
	{
		SkillTime += DeltaTime;
		if (SkillTime > 5.0f)
		{
			bIsSkillDelay = false;
			SkillTime = 0;
		}
	}

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
	default:
		break;
	}
}

void UWBFSMComponent::ChangeState(EMonsterState State)
{
	MState = State;
	StateTime = 0;
}

