// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBWeaponBase.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WBGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"


// Sets default values
AWBPlayerBase::AWBPlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->MaxAcceleration = 1000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->TargetArmLength = 4000.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 55.0f;


	Box1 = CreateDefaultSubobject<USceneComponent>(TEXT("Box1"));
	Box1->SetupAttachment(RootComponent);

	Box2 = CreateDefaultSubobject<USceneComponent>(TEXT("Box2"));
	Box2->SetupAttachment(RootComponent);

	Box3 = CreateDefaultSubobject<USceneComponent>(TEXT("Box3"));
	Box3->SetupAttachment(RootComponent);

	Box4 = CreateDefaultSubobject<USceneComponent>(TEXT("Box4"));
	Box4->SetupAttachment(RootComponent);

	Box5 = CreateDefaultSubobject<USceneComponent>(TEXT("Box5"));
	Box5->SetupAttachment(RootComponent);

	MonsterSpawnPosition1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos1"));
	MonsterSpawnPosition1->SetRelativeLocation(FVector(0, 1000, 0));
	MonsterSpawnPosition1->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition1);

	MonsterSpawnPosition2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos2"));
	MonsterSpawnPosition2->SetRelativeLocation(FVector(0, -1000, 0));
	MonsterSpawnPosition2->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition2);

	MonsterSpawnPosition3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos3"));
	MonsterSpawnPosition3->SetRelativeLocation(FVector(1000, 0, 0));
	MonsterSpawnPosition3->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition3);

	MonsterSpawnPosition4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos4"));
	MonsterSpawnPosition4->SetRelativeLocation(FVector(-1000, 0, 0));
	MonsterSpawnPosition4->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition4);

	bAutoMode = false;
	ClosestDistance = FLT_MAX;
	bIsAttacking = false;
}

// Called when the game starts or when spawned
void AWBPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	ConfigureInputMapping();
	MyPlayerController = Cast<APlayerController>(GetController());
	
	if (MyPlayerController)
	{
		MyPlayerController->bShowMouseCursor = true;
	}

	if (Box1 && ChampionOnlyWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("ChampionOnlyWeapon!!!!!!!!!!!!!!!!"));

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnedWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(ChampionOnlyWeapon, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (SpawnedWeapon)
		{
			SpawnedWeapon->AttachToComponent(Box1, FAttachmentTransformRules::SnapToTargetIncludingScale);
			SpawnedWeapon->OwnerCharacter = this;
		}
	}

	DefaultAttackSettings();

	AWBGameMode* GM = Cast<AWBGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GM))
	{
		GM->Players.Emplace(this);
	}


}

// Called every frame
void AWBPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWBPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Move)
		{
			EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AWBPlayerBase::Move);
		}
		if (IA_Skill_E)
		{
			EnhancedInputComponent->BindAction(IA_Skill_E, ETriggerEvent::Started, this, &AWBPlayerBase::SkillE);
		}
		if (IA_Skill_R)
		{
			EnhancedInputComponent->BindAction(IA_Skill_R, ETriggerEvent::Started, this, &AWBPlayerBase::SkillR);
		}
		if (IA_AutoMode)
		{
			EnhancedInputComponent->BindAction(IA_AutoMode, ETriggerEvent::Started, this, &AWBPlayerBase::ToggleAutoMode);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AWBPlayerBase::ConfigureInputMapping()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (IsValid(PlayerController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMapping)
			{
				Subsystem->AddMappingContext(InputMapping, 0);
			}
		}
	}
}


void AWBPlayerBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator MyRotation = GetControlRotation();
		const FRotator YawRotation(0, MyRotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWBPlayerBase::SkillE()
{
	// Skill E logic here
}

void AWBPlayerBase::SkillR()
{
	// Skill R logic here
}

void AWBPlayerBase::ToggleAutoMode()
{

	bAutoMode = !bAutoMode;
	DefaultAttackSettings();
}

void AWBPlayerBase::AutomaticAiming()
{
	if (!bAutoMode)
	{
		return;
	}
	ClosestDistance = FLT_MAX;
	ClosestEnemy = nullptr;

	FVector Start = GetActorLocation();
	FVector End = Start;
	float Radius = 500.0f;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Radius), CollisionParams);

	if (bHit && OutHit.GetActor())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), OutHit.GetActor()->GetClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			float Distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestEnemy = Actor;
			}
		}

		if (ClosestEnemy)
		{
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ClosestEnemy->GetActorLocation());
			SetActorRotation(TargetRotation);
		}
	}
}

void AWBPlayerBase::CheckAttack()
{
	UE_LOG(LogTemp, Error, TEXT("2 CheckAttack!!!"));

	if (!bIsAttacking)
	{
		bIsAttacking = true;
		AttackFire();
	}

}

void AWBPlayerBase::AttackFire()
{

	if (bIsAttacking)
	{
		UE_LOG(LogTemp, Error, TEXT("3 AttackFire!!!"));
		GetCharacterMovement()->bOrientRotationToMovement = false;
		CursorHitAiming();
		if (SpawnedWeapon)
		{
			SpawnedWeapon->Fire();
		}
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		//bIsAttacking = false;
		UE_LOG(LogTemp, Error, TEXT("7 AttackFire Clear!!!"));
		
		GetWorld()->GetTimerManager().ClearTimer(FTimerHandle_CursorAiming);

	}
}

void AWBPlayerBase::CursorHitAiming()
{
	if (!bAutoMode)
	{
		UE_LOG(LogTemp, Error, TEXT("4. CursorHitAiming Check!!!!!!!"));

		if (MyPlayerController)
		{
			FHitResult HitResult;
			
			if (MyPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
			{
				FVector TargetLocation = HitResult.Location;
				FVector ActorLocation = GetActorLocation();
	

				UE_LOG(LogTemp, Warning, TEXT("TargetLocation X: %f, Y: %f"), TargetLocation.X, TargetLocation.Y);
				FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(FVector(ActorLocation.X, ActorLocation.Y, 0.0f), FVector(TargetLocation.X, TargetLocation.Y, 0.0f));
				SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
			}
		}
	}


}

void AWBPlayerBase::DefaultAttackSettings()
{
	UE_LOG(LogTemp, Error, TEXT("1. DefaultAttackSettings Check!!!!!!!"));

	if (bAutoMode)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(FTimerHandle_CheckAttack))
		{
			GetWorld()->GetTimerManager().ClearTimer(FTimerHandle_CheckAttack);
		}

		GetCharacterMovement()->bOrientRotationToMovement = false;

		if (!GetWorld()->GetTimerManager().IsTimerActive(FTimerHandle_AutomaticAiming))
		{
			GetWorld()->GetTimerManager().SetTimer(FTimerHandle_AutomaticAiming, this, &AWBPlayerBase::AutomaticAiming, 0.01f, true);
		}
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(FTimerHandle_AutomaticAiming))
		{
			GetWorld()->GetTimerManager().ClearTimer(FTimerHandle_AutomaticAiming);
		}

		GetCharacterMovement()->bOrientRotationToMovement = true;

		if (!GetWorld()->GetTimerManager().IsTimerActive(FTimerHandle_CheckAttack))
		{
			GetWorld()->GetTimerManager().SetTimer(FTimerHandle_CheckAttack, this, &AWBPlayerBase::CheckAttack, 2.0f, true);

		}

		//UKismetSystemLibrary::K2_ClearTimer(this, TEXT("AutomaticAiming"));

		//GetCharacterMovement()->bOrientRotationToMovement = true;
		//UKismetSystemLibrary::K2_SetTimer(this, "CheckAttack", 2.0f, true);
	}
}