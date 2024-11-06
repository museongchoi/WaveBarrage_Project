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

	// ...
	
}

void UWBFSMComponent::IdleState()
{
	// 잠시 대기
}

void UWBFSMComponent::MoveState()
{
	//타겟 플레이어 방향으로 이동
	//이동 후 CanAttack 이 true 이고 거리가 사거리 안이라면 AttackState
	//아니면서 지속 시간이 끝나면 IdleState
}

void UWBFSMComponent::AttackState()
{
	//원거리 , 보스 몬스터 전용
	//투사체 발사 후 일정 시간 지나면 IdleState
}

void UWBFSMComponent::SkillState()
{
	// 보스 전용
	// 워닝 사인 이후 콜리전 검사해서 범위 내의 플레이어에게 데미지
	// 일정 시간 지나면 IdleState
}

void UWBFSMComponent::DieState()
{
	// Destroy Actor 진행하는 동안 움직이지 않도록 하기 위함
}


// Called every frame
void UWBFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWBFSMComponent::ChangeState()
{
}

