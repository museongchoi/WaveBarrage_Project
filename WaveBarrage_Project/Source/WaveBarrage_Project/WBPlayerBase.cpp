// Fill out your copyright notice in the Description page of Project Settings.


#include "WBPlayerBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WBWeaponBase.h"
#include "WeaponJinx.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WBGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
//#include "Engine/EngineTypes.h"
#include "WBMonsterBase.h"

// Sets default values
AWBPlayerBase::AWBPlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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


	WeaponPotionComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponPotion"));
	WeaponPotionComponent->SetupAttachment(GetMesh());

	MonsterSpawnPosition1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos1"));
	MonsterSpawnPosition1->SetRelativeLocation(FVector(0, 3000, 0));
	MonsterSpawnPosition1->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition1);

	MonsterSpawnPosition2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos2"));
	MonsterSpawnPosition2->SetRelativeLocation(FVector(0, -3000, 0));
	MonsterSpawnPosition2->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition2);

	MonsterSpawnPosition3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos3"));
	MonsterSpawnPosition3->SetRelativeLocation(FVector(3000, 0, 0));
	MonsterSpawnPosition3->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition3);

	MonsterSpawnPosition4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pos4"));
	MonsterSpawnPosition4->SetRelativeLocation(FVector(-3000, 0, 0));
	MonsterSpawnPosition4->SetupAttachment(RootComponent);
	MonsterSpawnPositions.Emplace(MonsterSpawnPosition4);

	bAutoMode = false;
	ClosestDistance = FLT_MAX;
}

// Called when the game starts or when spawned
void AWBPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	ConfigureInputMapping();
	MyPlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if (MyPlayerController)
	{
		MyPlayerController->bShowMouseCursor = true;
	}

	if (WeaponPotionComponent && ChampionOnlyWeapon)
	{
		

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnedWeapon = GetWorld()->SpawnActor<AWeaponJinx>(ChampionOnlyWeapon, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (SpawnedWeapon)
		{
			SpawnedWeapon->AttachToComponent(WeaponPotionComponent, FAttachmentTransformRules::KeepWorldTransform);
			SpawnedWeapon->OwnerCharacter = this;
			EquippedWeapons.Add(SpawnedWeapon);
		}
	}

	AWBGameMode* GM = Cast<AWBGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GM))
	{
		GM->Players.Emplace(this);
	}

	// 게임 시작 시 초기 방향을 마우스 커서 쪽으로 설정
	if (!bAutoMode)
	{
		CursorHitAiming();
	}

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(FTimerHandle_AttackFire, this, &AWBPlayerBase::AttackFire, 3.0f, true);
	}
}

// Called every frame
void AWBPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWBPlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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
		//UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
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
		FVector Direction = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;

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
	if (bAutoMode)
	{
		bAutoMode = false;
	} 
	else
	{
		bAutoMode = true;
	}
}

// 가장 가까운 몬스터 자동 타겟팅
void AWBPlayerBase::AutomaticAiming()
{
	if (!bAutoMode)
	{
		return;
	}

	ClosestDistance = FLT_MAX;
	ClosestEnemy = nullptr;

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 1500.0f;
	float Radius = 1500.0f;

	TArray<FHitResult> OutHits;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(
		OutHits,
		Start, 
		End, 
		FQuat::Identity, 
		ECC_GameTraceChannel1, 
		FCollisionShape::MakeSphere(Radius), 
		CollisionParams
	);

	

	// 디버그용 구체 표시
	DrawDebugSphere(GetWorld(), Start, Radius, 12, FColor::Red, false, 1.f);
	// 디버그용 구체 표시 (한 프레임 동안 표시)
	//DrawDebugSphere(GetWorld(), Start, Radius, 12, FColor::Red, false, -1.f, 0, 1.0f);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();

			if (AWBMonsterBase* TargetMonster = Cast<AWBMonsterBase>(HitActor))
			{
				

				float Distance = FVector::Dist(TargetMonster->GetActorLocation(), GetActorLocation());
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestEnemy = TargetMonster;
				}
			}
		}


		if (ClosestEnemy)
		{
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ClosestEnemy->GetActorLocation());
			GetMesh()->SetWorldRotation(FRotator(0.0f, TargetRotation.Yaw - 90.f, 0.0f));
			// 멀티캐스트로 모든 클라이언트에 회전 정보를 동기화
			ServerSetOrientation(TargetRotation.Yaw, false);
		}
	}
}

// 마우스 커서 위치
void AWBPlayerBase::CursorHitAiming()
{
	if (!bAutoMode)
	{
		if (MyPlayerController)
		{
			FHitResult HitResult;

			if (MyPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
			{
				FVector TargetLocation = HitResult.Location;
				FVector ActorLocation = GetActorLocation();

				FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(FVector(ActorLocation.X, ActorLocation.Y, 0.0f), FVector(TargetLocation.X, TargetLocation.Y, 0.0f));
				
				if (!HasAuthority())
				{
					ServerSetOrientation(NewRotation.Yaw, false);
				}
				else
				{
					MulticastSetOrientation(NewRotation.Yaw, false);
				}
			}
		}
	}
}

void AWBPlayerBase::ServerSetOrientation_Implementation(float NewRotation, bool bOrientRotationToMovement)
{
	GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
	if (NewRotation != 999.99f)
	{
		GetMesh()->SetWorldRotation(FRotator(0.0f, NewRotation - 90.f, 0.0f));
	}

	// 서버에서 회전 설정 후 모든 클라이언트에 동기화 요청
	MulticastSetOrientation(NewRotation, bOrientRotationToMovement);
}

void AWBPlayerBase::MulticastSetOrientation_Implementation(float NewRotation, bool bOrientRotation)
{
	GetCharacterMovement()->bOrientRotationToMovement = bOrientRotation;
	GetMesh()->SetWorldRotation(FRotator(0.0f, NewRotation - 90.f, 0.0f));
	UE_LOG(LogTemp, Warning, TEXT("NewRotation : %f"), NewRotation);

}

void AWBPlayerBase::AttackFire()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("HasAuthority Call"));
		ServerSetOrientation(999.99f, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("client Call"));

	}

	if (!bAutoMode)
	{
		if (!HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("client Call"));

		}

		CursorHitAiming();
	}
	else if (bAutoMode)
	{
		AutomaticAiming();
	}

	if (HasAuthority())
	{
		SpawnedWeapon->Fire();
		MulticastAttackFire();
	}
	else
	{
		// 클라이언트에서도 로컬에서 공격 시도 시, 메쉬 회전 즉시 반영
		if (SpawnedWeapon)
		{
			ServerAttackFire();  // 서버에 공격 요청
		}
	}
}

void AWBPlayerBase::ServerAttackFire_Implementation()
{
	if (SpawnedWeapon)
	{
		MulticastAttackFire(); // 모든 클라이언트에 발사 동기화
	}
}

void AWBPlayerBase::MulticastAttackFire_Implementation()
{
	if (SpawnedWeapon)
	{
		SpawnedWeapon->Fire();
	}
}
