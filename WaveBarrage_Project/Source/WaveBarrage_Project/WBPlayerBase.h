// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WBPlayerBase.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class AWBMonsterBase;
class AActor;
class AWBWeaponBase;
class AWeaponJinx;

UCLASS()
class WAVEBARRAGE_PROJECT_API AWBPlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWBPlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ConfigureInputMapping();

	// 공격 사용 함수
	UFUNCTION()
	void AutomaticAiming();

	UFUNCTION()
	void CursorHitAiming();

	UFUNCTION()
	void AttackFire();

	UFUNCTION(Server, Reliable)
	void ServerSetOrientation(float NewRotation, bool bOrientRotationToMovement);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetOrientation(float NewRotation, bool bOrientRotation);

	void Move(const FInputActionValue& Value);

	void SkillE();

	void SkillR();

	void ToggleAutoMode();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> WeaponPotionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MonsterSpawnPosition1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MonsterSpawnPosition2;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MonsterSpawnPosition3;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MonsterSpawnPosition4;

	UPROPERTY(VisibleAnywhere)
	TArray<USceneComponent*> MonsterSpawnPositions;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_E;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Skill_R;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_AutoMode;

	UPROPERTY()
	TObjectPtr<APlayerController> MyPlayerController;

	// 캐릭터 각 전용 무기
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWBWeaponBase> ChampionOnlyWeapon;

	// 일반 무기들을 부착할 Box 배열 (Blueprint에서 설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<AWBWeaponBase>> WeaponAttachBoxes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ClosestDistance;

	UPROPERTY()
	TObjectPtr<AActor> ClosestEnemy;

	UPROPERTY()
	AWeaponJinx* SpawnedWeapon;

	UPROPERTY()
	AWBWeaponBase* GeneralSpawnedWeapon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, ReplicatedUsing = OnRep_EquippedWeapons)
	TArray<AWBWeaponBase*> EquippedWeapons;

	FTimerHandle FTimerHandle_AttackFire;
	FTimerHandle FTimerHandle_AutomaticAiming;
	FTimerHandle FTimerHandle_CursorAiming;

	UFUNCTION()
	void OnRep_EquippedWeapons();

	// 고유 플레이어 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 PlayerID;

	// 데미지 적용 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamageToPlayer(int32 Damage);

	// 데미지 적용 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyDamageToGameState(int32 ActualDamage);

	// 서버 RPC: 데미지 적용 요청
	UFUNCTION(Server, Reliable)
	void Server_ApplyDamage(int32 Damage);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
