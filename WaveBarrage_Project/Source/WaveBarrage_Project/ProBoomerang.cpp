// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBoomerang.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WBMonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AProBoomerang::AProBoomerang()
{
	bReplicates = true;
	LifeTime = 4.0f;
	Acceleration = -0.25f;

}

void AProBoomerang::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
	
}

void AProBoomerang::MoveForward(FVector Direction,float DeltaTime)
{
	Direction = Direction * ProjectileSpeed * DeltaTime;
	AddActorWorldOffset(Direction , true);
}

void AProBoomerang::MoveBackWard(FVector Direction)
{
	Direction = AttackDirection * -1.0f * ProjectileSpeed;
	AddActorWorldOffset(Direction);
}

void AProBoomerang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority())
	{
		Time += DeltaTime;

		AttackDirection = GetActorForwardVector();

		ProjectileSpeed += Acceleration;

		MoveForward(AttackDirection, Time);
	}

}

void AProBoomerang::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	
	if (OtherActor && OtherActor != this)
	{
		if (HasAuthority() && CanCollision == true)
		{
			

			AWBMonsterBase* Monster = Cast<AWBMonsterBase>(OtherActor);
			if (Monster)
			{
				UGameplayStatics::ApplyDamage(Monster, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
			
			}
		}
	}
}

void AProBoomerang::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProBoomerang, AttackDirection);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProBoomerang, ProjectileSpeed);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProBoomerang, Time);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProBoomerang, Acceleration);
}
