// Fill out your copyright notice in the Description page of Project Settings.


#include "WBProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WBMonsterBase.h"

// Sets default values
AWBProjectileBase::AWBProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetCollisionProfileName(TEXT("ProjectileProfile"));
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AWBProjectileBase::OnSphereOverlapBegin);
	RootComponent = SphereCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SphereCollision);

}

// Called when the game starts or when spawned
void AWBProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWBProjectileBase::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Error, TEXT("OnSphereOverlapBegin Check!!!!!!!"));

		AWBMonsterBase* Monster = Cast<AWBMonsterBase>(OtherActor);
		if (Monster)
		{
			UGameplayStatics::ApplyDamage(Monster, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		}
	}
}

void AWBProjectileBase::SetDamage(int32 InDamage)
{
	Damage = InDamage;
}

// Called every frame
void AWBProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

