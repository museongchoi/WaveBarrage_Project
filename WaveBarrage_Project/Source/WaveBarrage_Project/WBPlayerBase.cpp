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
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "WBGameState.h"
#include "WBPlayerState.h"


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
	PlayerID = 0; // 초기 값, 게임 시작 시 할당 필요
}

// Called when the game starts or when spawned
void AWBPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AWBGameState* GS = GetWorld()->GetGameState<AWBGameState>();
		if (GS)
		{
			PlayerID = GS->AssignPlayerID();
			if (PlayerID != -1)
			{
				UE_LOG(LogTemp, Warning, TEXT("Assigned PlayerID %d"), PlayerID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to assign PlayerID"));
			}
		}

		//// 서버에서만 PlayerID를 할당합니다.
		//// 예를 들어, 플레이어의 Controller ID를 사용할 수 있습니다.
		//APlayerController* PC = Cast<APlayerController>(GetController());
		//if (PC)
		//{
		//	PlayerID = PC->GetLocalPlayer()->GetControllerId() + 1; // 간단한 예시
		//	UE_LOG(LogTemp, Log, TEXT("Assigned PlayerID %d"), PlayerID);
		//}

		//// GameState에 PlayerID 등록 (필요 시)
		//AWBGameState* GS = GetWorld()->GetGameState<AWBGameState>();
		//if (GS)
		//{
		//	bool bFound = false;
		//	for (auto& PS : GS->PlayerStates)
		//	{
		//		if (PS.PlayerID == 0)
		//		{
		//			PS.PlayerID = PlayerID;
		//			PS.HP = 100;
		//			bFound = true;
		//			UE_LOG(LogTemp, Warning, TEXT("PlayerState 등록: PlayerID = %d, HP = %d"), PlayerID, PS.HP);
		//			break;
		//		}
		//	}

		//	if (!bFound)
		//	{
		//		// PlayerStates 배열에 빈 슬롯이 없을 경우 처리
		//		UE_LOG(LogTemp, Warning, TEXT("PlayerStates 배열에 빈 슬롯이 없습니다."));
		//	}
		//}
	}
	
	if (IsLocallyControlled())
	{
		ConfigureInputMapping();
		MyPlayerController = Cast<APlayerController>(GetController());

		if (MyPlayerController)
		{
			MyPlayerController->bShowMouseCursor = true;
		}
		if (!MyPlayerController)
		{
			UE_LOG(LogTemp, Error, TEXT("MyPlayerController is null on %s"), *GetName());
		}
	}
	
	if (HasAuthority())
	{
		if (WeaponPotionComponent && ChampionOnlyWeapon)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			SpawnedWeapon = GetWorld()->SpawnActor<AWeaponJinx>(ChampionOnlyWeapon, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

			if (SpawnedWeapon)
			{
				// 부착 규칙을 정의합니다.
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

				// 캐릭터 메쉬에 부착 (소켓 없이)
				SpawnedWeapon->AttachToComponent(this->GetMesh(), AttachmentRules);
				SpawnedWeapon->OwnerCharacter = this;
				EquippedWeapons.Add(SpawnedWeapon);
				OnRep_EquippedWeapons();
				UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter : %s"), *SpawnedWeapon->OwnerCharacter->GetName());
			}
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


	GetWorld()->GetTimerManager().SetTimer(FTimerHandle_AttackFire, this, &AWBPlayerBase::AttackFire, 3.0f, true);

}

// Called every frame
void AWBPlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWBPlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWBPlayerBase,EquippedWeapons);
	DOREPLIFETIME(AWBPlayerBase, PlayerID);
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

void AWBPlayerBase::OnRep_EquippedWeapons()
{
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

			if (!HasAuthority())
			{
				ServerSetOrientation(TargetRotation.Yaw, false);
			}
			else
			{
				MulticastSetOrientation(TargetRotation.Yaw, false);
			}
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

				UE_LOG(LogTemp, Warning, TEXT("TargetLocation : %s"), *TargetLocation.ToString())

				FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(FVector(ActorLocation.X, ActorLocation.Y, 0.0f), FVector(TargetLocation.X, TargetLocation.Y, 0.0f));

				
				if (IsLocallyControlled())
				{
					ServerSetOrientation(NewRotation.Yaw, false);
				}
			}
		}
	}
}

void AWBPlayerBase::ServerSetOrientation_Implementation(float NewRotation, bool bOrientRotationToMovement)
{
	// 서버에서 클라이언트로 회전 정보 전송
	MulticastSetOrientation(NewRotation, bOrientRotationToMovement);

}

void AWBPlayerBase::MulticastSetOrientation_Implementation(float NewRotation, bool bOrientRotation)
{

	GetCharacterMovement()->bOrientRotationToMovement = bOrientRotation;
	//GetMesh()->SetWorldRotation(FRotator(0.0f, NewRotation - 90.0f, 0.0f));

	FRotator NewMeshRotation(0.0f, NewRotation - 90.0f, 0.0f);
	GetMesh()->SetWorldRotation(NewMeshRotation);

	// 무기의 회전도 캐릭터 메쉬의 회전과 일치시킴
	if (SpawnedWeapon && SpawnedWeapon->ProjectileSpawnPoint)
	{
		FRotator SpawnPointRotation = FRotator(0.0f, NewRotation, 0.0f);
		SpawnedWeapon->ProjectileSpawnPoint->SetWorldRotation(SpawnPointRotation);
	}
}

void AWBPlayerBase::AttackFire()
{
	if (IsLocallyControlled())
	{
		if (!bAutoMode)
		{
			CursorHitAiming();
		}
		else if (bAutoMode)
		{
			AutomaticAiming();
		}
	}

	if (HasAuthority())
	{
		// 공격 로직 및 서버 측 처리를 여기서 수행
		if (SpawnedWeapon)
		{
			SpawnedWeapon->Fire();
		}
	}
}

void AWBPlayerBase::ApplyDamageToPlayer(int32 Damage)
{
	if (HasAuthority())
	{
		// 서버에서 직접 데미지 적용
		int32 ActualDamage = FMath::Max(Damage, 0);
		ApplyDamageToGameState(ActualDamage);
	}
	else
	{
		// 클라이언트에서 서버에 데미지 적용 요청
		Server_ApplyDamage(Damage);
	}
}

void AWBPlayerBase::ApplyDamageToGameState(int32 ActualDamage)
{
	AWBGameState* GS = GetWorld()->GetGameState<AWBGameState>();
	if (GS)
	{
		GS->UpdatePlayerHP(PlayerID, ActualDamage);
	}
}


void AWBPlayerBase::Server_ApplyDamage_Implementation(int32 Damage)
{
	ApplyDamageToPlayer(Damage);
}

float AWBPlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Log, TEXT("AWBPlayerBase::TakeDamage : DamageAmount = %f"), DamageAmount);

	// 서버에서만 데미지를 처리
	if (!HasAuthority())
	{
		return 0.0f;
	}

	// PlayerState에서 Armor 값을 가져옴
	AWBPlayerState* PS = GetPlayerState<AWBPlayerState>();
	int32 ArmorValue = 0;

	if (PS)
	{
		ArmorValue = PS->Armor;
	}

	// 실제 데미지 계산
	int32 ActualDamage = FMath::Max(static_cast<int32>(DamageAmount) - ArmorValue, 0);

	if (ActualDamage > 0)
	{
		ApplyDamageToGameState(ActualDamage);
	}

	return Damage;
}