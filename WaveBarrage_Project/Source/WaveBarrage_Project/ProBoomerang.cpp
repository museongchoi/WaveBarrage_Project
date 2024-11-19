// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBoomerang.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProBoomerang::AProBoomerang()
{
	ProjectileSpeed = 10.0f;
	LifeTime = 4.0f;
	Acceleration = -0.25f;
}

void AProBoomerang::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
	
}

void AProBoomerang::MoveForward(FVector Direction)
{
	Direction = Direction * ProjectileSpeed;
	AddActorWorldOffset(Direction);
}

void AProBoomerang::MoveBackWard(FVector Direction)
{
	Direction = AttackDirection * -1.0f * ProjectileSpeed;
	AddActorWorldOffset(Direction);
}

void AProBoomerang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time += DeltaTime;
		
	AttackDirection = GetActorForwardVector();
	
	ProjectileSpeed += Acceleration;

	MoveForward(AttackDirection);
	

}
