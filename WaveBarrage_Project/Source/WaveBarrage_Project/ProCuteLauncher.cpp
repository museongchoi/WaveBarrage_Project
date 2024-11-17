// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCuteLauncher.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProCuteLauncher::AProCuteLauncher()
{
	//ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 2000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;


	LifeTime = 2.0f;
}

void AProCuteLauncher::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);

}
