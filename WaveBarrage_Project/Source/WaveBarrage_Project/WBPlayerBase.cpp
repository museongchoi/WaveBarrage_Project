// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerBase.h"
#include "WBGameMode.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "WBWeaponBase.h"


// Sets default values
AWBPlayerBase::AWBPlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
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


	Box1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box1"));
	Box1->SetupAttachment(RootComponent);

	Box2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box2"));
	Box2->SetupAttachment(RootComponent);

	Box3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box3"));
	Box3->SetupAttachment(RootComponent);

	Box4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box4"));
	Box4->SetupAttachment(RootComponent);

	Box5 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box5"));
	Box5->SetupAttachment(RootComponent);

	MonsterSpawnPosition1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos1"));
	MonsterSpawnPosition1->SetRelativeLocation(FVector(0, 1000, 0));
	MonsterSpawnPosition1->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition1);

	MonsterSpawnPosition2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos2"));
	MonsterSpawnPosition2->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition2);

	MonsterSpawnPosition3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos3"));
	MonsterSpawnPosition3->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition3);

	MonsterSpawnPosition4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos4"));
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

	if (Box1)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ChampionOnlyWeapon = GetWorld()->SpawnActor<AWBWeaponBase>(AWBWeaponBase::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (ChampionOnlyWeapon)
		{
			ChampionOnlyWeapon->AttachToComponent(Box1, FAttachmentTransformRules::SnapToTargetIncludingScale);
			ChampionOnlyWeapon->OwnerCharacter = this;
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
		UKismetSystemLibrary::K2_SetTimer(this, "CursorHitAiming", 0.01f, true);
		ChampionOnlyWeapon->Fire();
	}
	else
	{
		CursorHitAiming();

	}
	bIsAttacking = false;
}

void AWBPlayerBase::CursorHitAiming()
{
	if (MyPlayerController)
	{
		FVector WorldLocation, WorldDirection;
		MyPlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		CursorRotation = WorldDirection.Rotation();
	}
}

void AWBPlayerBase::DefaultAttackSettings()
{
	if (bAutoMode)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		UKismetSystemLibrary::K2_SetTimer(this, "AutomaticAiming", 0.01f, true);

	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		UKismetSystemLibrary::K2_SetTimer(this, "CheckAttack", 2.0f, true);

	}
}