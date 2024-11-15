// Fill out your copyright notice in the Description page of Project Settings.


#include "ProWhirlwindBlade.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AProWhirlwindBlade::AProWhirlwindBlade()
{
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
