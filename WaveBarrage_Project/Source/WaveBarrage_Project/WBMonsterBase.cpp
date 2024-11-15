// Fill out your copyright notice in the Description page of Project Settings.


#include "WBMonsterBase.h"
#include "Components/SphereComponent.h"
#include "WBFSMComponent.h"
#include "WBItemBase.h"
#include "WBMonsterGroup.h"

// Sets default values
AWBMonsterBase::AWBMonsterBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(RootComponent);

	FSMComp = CreateDefaultSubobject<UWBFSMComponent>(TEXT("FSM Comp"));
}

// Called when the game starts or when spawned
void AWBMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWBMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWBMonsterBase::Attack()
{
	
}

