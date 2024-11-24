// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerState.h"
#include "Net/UnrealNetwork.h"

AWBPlayerState::AWBPlayerState()
{
    Armor = 10;
}


int32 AWBPlayerState::GetDamageMultiplier() const
{
    // 기본 데미지 배율은 1.0 (즉, 데미지 증가 없음)
    int32 DamageMultiplier = 1;

    // 아이템 레벨 맵에서 "Damage" 키에 해당하는 레벨을 찾고, 없으면 기본값 0으로 설정
    const int16* DamageLevelPtr = ItemLevel.Find("Damage");
    int16 DamageLevel = (DamageLevelPtr) ? *DamageLevelPtr : 0;

    // 각 레벨당 10%씩 데미지 증가
    DamageMultiplier += 0.1f * DamageLevel;

    return DamageMultiplier;
}

void AWBPlayerState::SetPlayerState(EPlayerState NewState)
{
    if (HasAuthority())
    {
        CurrentState = NewState;
        OnRep_PlayerState();
    }
}

void AWBPlayerState::OnRep_PlayerState()
{

}

void AWBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AWBPlayerState, CurrentState);
}
