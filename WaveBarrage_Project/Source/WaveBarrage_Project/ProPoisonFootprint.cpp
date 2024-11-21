// Fill out your copyright notice in the Description page of Project Settings.


#include "ProPoisonFootprint.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WBMonsterBase.h"
#include "Kismet/GameplayStatics.h"


AProPoisonFootprint::AProPoisonFootprint()
{
	bReplicates = true;
	LifeTime = 4.0f;
}

void AProPoisonFootprint::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);

}


void AProPoisonFootprint::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && OtherActor != this)
	{
		if (HasAuthority() && CanCollision == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnSphereOverlapBegin Check!!!!!!!"));

			AWBMonsterBase* Monster = Cast<AWBMonsterBase>(OtherActor);
			if (Monster)
			{
				UGameplayStatics::ApplyDamage(Monster, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

			}
		}
	}
}