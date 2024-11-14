// Fill out your copyright notice in the Description page of Project Settings.


#include "WBProjectileBase.h"

// Sets default values
AWBProjectileBase::AWBProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWBProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWBProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

