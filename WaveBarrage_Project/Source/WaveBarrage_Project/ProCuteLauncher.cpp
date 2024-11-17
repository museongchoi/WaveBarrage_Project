// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCuteLauncher.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProCuteLauncher::AProCuteLauncher()
{
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;


	LifeTime = 3.0f;
}

void AProCuteLauncher::BeginPlay()
{
	SetLifeSpan(LifeTime);

}
