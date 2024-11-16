// Fill out your copyright notice in the Description page of Project Settings.


#include "ProPoisonFootprint.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"


AProPoisonFootprint::AProPoisonFootprint()
{

}

void AProPoisonFootprint::BeginPlay()
{
	SetLifeSpan(5.0f);

	if (StaticMesh)
	{
		StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AProPoisonFootprint::OnComponentBeginOverlap);
	}
}

void AProPoisonFootprint::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (ACharacter* MyCharacter = Cast<ACharacter>(OtherActor))
	{
		AddDamage();
	}
}

void AProPoisonFootprint::AddDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("Damaged"));
}
