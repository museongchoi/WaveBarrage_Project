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

void AProPoisonFootprint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // 모든 타이머 정리
    for (auto& Elem : TimerHandles)
    {
        GetWorld()->GetTimerManager().ClearTimer(Elem.Value);
    }
    TimerHandles.Empty();
    OverlappedMonsters.Empty();
}


void AProPoisonFootprint::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if (OtherActor && OtherActor != this)
    {
        if (HasAuthority() && CanCollision)
        {
            AWBMonsterBase* Monster = Cast<AWBMonsterBase>(OtherActor);
            if (Monster && !OverlappedMonsters.Contains(Monster))
            {
                OverlappedMonsters.Add(Monster);

                FTimerHandle TimerHandle;
                TimerHandles.Add(Monster, TimerHandle);

                FTimerDelegate TimerDel;
                TimerDel.BindUFunction(this, FName("DamageTick"), Monster);

                GetWorld()->GetTimerManager().SetTimer(
                    TimerHandles[Monster], TimerDel, 0.25f, true, 0.0f);
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
        FTimerHandle* TimerHandle = TimerHandles.Find(OtherActor);
        if (TimerHandle)
        {
            GetWorld()->GetTimerManager().ClearTimer(*TimerHandle);
            TimerHandles.Remove(OtherActor);
        }
        if(AWBMonsterBase* OverlapMonster = Cast<AWBMonsterBase>(OtherActor))
        OverlappedMonsters.Remove(OverlapMonster);
    }
}
