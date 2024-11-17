// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBoomerang.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProBoomerang::AProBoomerang()
{
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 0.0f;
	ProjectileMovementComponent->ProjectileGravityScale=0.0f;
	ProjectileSpeed = 10.0f;
	LifeTime = 4.0f;

}

void AProBoomerang::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
	Acceleration = -0.25f;
}

void AProBoomerang::MoveForward(FVector Direction)
{
	Direction = AttackDirection * ProjectileSpeed;
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
