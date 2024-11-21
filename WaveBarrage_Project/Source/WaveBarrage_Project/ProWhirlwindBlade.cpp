// Fill out your copyright notice in the Description page of Project Settings.


#include "ProWhirlwindBlade.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "WBMonsterBase.h"

AProWhirlwindBlade::AProWhirlwindBlade()
{
	bReplicates = true;
	LifeTime = 2.0f;
	OrbitSpeed = 180.0f;
	OrbitRadius = 360.0f;
	
}

void AProWhirlwindBlade::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
}

void AProWhirlwindBlade::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	CurAngle +=  OrbitSpeed * DeltaSeconds;
	//CurAngle += SpawnLocationByRadians;
	float RadianAngle = UKismetMathLibrary::DegreesToRadians(CurAngle+SpawnLocationByRadians);

	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if (MyCharacter)
	{
		Center = MyCharacter->GetActorLocation();

		float X = cos(RadianAngle) * OrbitRadius;
		float Y = sin(RadianAngle) * OrbitRadius;

		FVector Position = Center + FVector(X, Y, 0);

		SetActorLocation(Position);
	}
}

void AProWhirlwindBlade::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
