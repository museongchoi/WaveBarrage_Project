// Fill out your copyright notice in the Description page of Project Settings.


#include "ProPoisonFootprint.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"



AProPoisonFootprint::AProPoisonFootprint()
{
	LifeTime = 4.0f;
}

void AProPoisonFootprint::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);

}


void AProPoisonFootprint::AddDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("Damaged"));
}
