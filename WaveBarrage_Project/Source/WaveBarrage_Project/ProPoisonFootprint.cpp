// Fill out your copyright notice in the Description page of Project Settings.


#include "ProPoisonFootprint.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WBMonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"



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
			AWBMonsterBase* Monster = Cast<AWBMonsterBase>(OtherActor);
			if (Monster)
			{
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("DamageTick"), Monster);

				GetWorld()->GetTimerManager().SetTimer(FTimerHandle_DamageTick, TimerDel, 0.25f, true);
			}
		}
	}
}

void AProPoisonFootprint::DamageTick(AActor* OtherActor)
{
	if (IsValid(OtherActor))
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
	}

	else
	{
		GetWorld()->GetTimerManager().ClearTimer(FTimerHandle_DamageTick);
	}
	
}
