// Fill out your copyright notice in the Description page of Project Settings.


#include "WBItemBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WBGameMode.h"

// Sets default values
AWBItemBase::AWBItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = Root;
	Root->OnComponentBeginOverlap.AddDynamic(this, &AWBItemBase::OnOverlapBegin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
// Called when the game starts or when spawned
void AWBItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWBItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWBItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		AWBGameMode* GM = Cast<AWBGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			if (OtherActor->ActorHasTag(TEXT("Player")))
			{
				switch (ItemType)
				{
				case EItemType::Exp1:
					GM->AddExp(1);
					break;
				case EItemType::Exp2:
					GM->AddExp(5);
					break;
				case EItemType::Exp3:
					GM->AddExp(10);
					break;
				case EItemType::Card:
					GM->LevelUp();
					break;
				case EItemType::Heal:
					break;
				default:
					break;
				}
				Destroy();
			}
		}
	}
}

