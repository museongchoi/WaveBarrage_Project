// Fill out your copyright notice in the Description page of Project Settings.


#include "WBMonsterBase.h"
#include "Components/SphereComponent.h"
#include "WBFSMComponent.h"
#include "WBItemBase.h"
#include "WBMonsterGroup.h"
#include "WBMonsterProjectile.h"

// Sets default values
AWBMonsterBase::AWBMonsterBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	SphereComp->SetSimulatePhysics(true);
	SphereComp->SetEnableGravity(false);
	SphereComp->GetBodyInstance()->bLockZTranslation = true;
	SphereComp->GetBodyInstance()->bLockXRotation = true;
	SphereComp->GetBodyInstance()->bLockYRotation = true;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AWBMonsterBase::OnSphereOverlapBegin);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(RootComponent);

	FSMComp = CreateDefaultSubobject<UWBFSMComponent>(TEXT("FSM Comp"));

	bReplicates = true;

	
}

// Called when the game starts or when spawned
void AWBMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AWBMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AWBMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (HasAuthority())
	{
		HP -= DamageAmount;

		if (HP <= 0)
		{
			GetWorld()->SpawnActor<AActor>(DropItem, GetActorLocation(), GetActorRotation());
			AWBMonsterGroup* OwnerGroup = Cast<AWBMonsterGroup>(GetOwner());
			if (IsValid(OwnerGroup))
			{
				OwnerGroup->RemoveMonster(this);
			}
			Destroy();
		}
	}
	return Damage;
}

void AWBMonsterBase::Attack()
{
	GetWorld()->SpawnActor<AActor>(MonsterProjectile, GetActorLocation(), GetActorRotation());
	//attack 애니메이션 실행
}

void AWBMonsterBase::Skill()
{

}

void AWBMonsterBase::SetTargetPlayer(AActor* Target)
{
	if (IsValid(FSMComp))
	{
		FSMComp->TargetPlayer = Target;
	}
}

void AWBMonsterBase::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (OtherActor->ActorHasTag(TEXT("Player")))
		{

		}
	}
}

