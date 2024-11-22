// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WBProjectileBase.h"
#include "ProWhirlwindBlade.generated.h"

/**
 * 
 */
UCLASS()
class WAVEBARRAGE_PROJECT_API AProWhirlwindBlade : public AWBProjectileBase
{
	GENERATED_BODY()

public:
	AProWhirlwindBlade();
	
protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    // 서버에서 위치 계산 및 업데이트
    void UpdatePosition();

    // 클라이언트로 위치 동기화
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_UpdatePosition(FVector NewPosition);
    void Multicast_UpdatePosition_Implementation(FVector NewPosition);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    FTimerHandle OrbitTimerHandle;

    // 현재 각도
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CurAngle;

    // 중심 위치
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FVector Center;





    // 회전 속도 (도/초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    float OrbitSpeed;

    // 회전 반경
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
    float OrbitRadius;

    // 생성 위치의 각도
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Replicated)
    float SpawnLocationByRadians;

    // 충돌 가능 여부
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Replicated)
    bool CanCollision;

 
};
