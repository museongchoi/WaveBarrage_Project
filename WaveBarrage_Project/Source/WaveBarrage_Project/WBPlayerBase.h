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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ConfigureInputMapping();

	void AutomaticAiming();
	void CheckAttack();
	void AttackFire();
	void CursorHitAiming();
	void DefaultAttackSettings();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<UBoxComponent> Box1;

	UPROPERTY()
	TObjectPtr<UBoxComponent> Box2;

	UPROPERTY()
	TObjectPtr<UBoxComponent> Box3;

	UPROPERTY()
	TObjectPtr<UBoxComponent> Box4;

	UPROPERTY()
	TObjectPtr<UBoxComponent> Box5;

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

	void Move(const FInputActionValue& Value);

	void SkillE();

	void SkillR();

	void ToggleAutoMode();

	UPROPERTY()
	TObjectPtr<APlayerController> MyPlayerController;

	UPROPERTY()
	TObjectPtr<UBoxComponent> ChampionOnlyWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ClosestDistance;

	UPROPERTY()
	TObjectPtr<AActor> ClosestEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator CursorRotation;


};
