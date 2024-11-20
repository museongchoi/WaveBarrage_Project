// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCuteLauncher.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WBMonsterBase.h"
#include "Kismet/GameplayStatics.h"



AProCuteLauncher::AProCuteLauncher()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
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

void AProCuteLauncher::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
				Destroy();
			}
		}
	}
}

