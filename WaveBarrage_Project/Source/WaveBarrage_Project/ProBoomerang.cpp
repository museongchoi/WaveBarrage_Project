// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBoomerang.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WBMonsterBase.h"
#include "Kismet/GameplayStatics.h"

AProBoomerang::AProBoomerang()
{
	bReplicates = true;
	ProjectileSpeed = 10.0f;
	LifeTime = 4.0f;
	Acceleration = -0.25f;
}

void AProBoomerang::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
	
}

void AProBoomerang::MoveForward(FVector Direction)
{
	Direction = Direction * ProjectileSpeed;
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

void AProBoomerang::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
