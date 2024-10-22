// Fill out your copyright notice in the Description page of Project Settings.

#include "Brawl.h"
#include "BrawlLogic.h"
#include "BrawlLogging.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "MovementScheme.h"

ABrawl::ABrawl()
{
	PrimaryActorTick.bCanEverTick = true;

	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Pivot);
	CA_Player = CreateDefaultSubobject<UChildActorComponent>(TEXT("Player"));
	CA_Player->SetupAttachment(RootComponent);
	CA_Enemy1 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Enemy1"));
	CA_Enemy1->SetupAttachment(RootComponent);
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FPCamera->SetupAttachment(CA_Player);

	BrawlLogic = CreateDefaultSubobject<UBrawlLogic>(TEXT("Brawl Logic Component"));
	MovementCmp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
}

void ABrawl::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(!InputMappingContext.IsNull());
	ULocalPlayer* LocalPlayer { Cast<APlayerController>(NewController)->GetLocalPlayer() };
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	Subsystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 0);
}

void ABrawl::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorTransform();
	Player = Cast<ABrawlerBase>(CA_Player->GetChildActor());
	Enemy1 = Cast<ABrawlerBase>(CA_Enemy1->GetChildActor());
	BrawlLogic->SetProperties(Player, Enemy1);

	UE_LOG(LogBrawl,Display,TEXT("%s - BeginPlay"),*GetNameSafe(this))
}

void ABrawl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const bool bPossessed{IsValid(Controller)};
	if (bPossessed && !(Player->IsKOd() || Enemy1->IsKOd()))
	{
		const FVector Direction{ MovementScheme->GetMovementDirection(GetActorLocation()) };
		AddMovementInput(Direction);

		const FRotator DesiredRotation{ GetOrientToMovementRotation() };
		if (!GetActorRotation().Equals(DesiredRotation, 0.01f))
		{
			SetActorRotation(DesiredRotation);
		}
	}
}

FRotator ABrawl::GetOrientToMovementRotation() const
{
	if (MovementCmp->Velocity.SquaredLength() < UE_KINDA_SMALL_NUMBER)
	{
		return GetActorRotation();
	}

	return FRotator(0.f,MovementCmp->Velocity.GetSafeNormal().Rotation().Yaw, 0.f);
}

void ABrawl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(IA_SelectDirection, ETriggerEvent::Triggered, this, &ABrawl::DirectionSelected);
	Input->BindAction(IA_BlockHeld, ETriggerEvent::Triggered, this, &ABrawl::BlockHeld);
	Input->BindAction(IA_BlockReleased, ETriggerEvent::Triggered, this, &ABrawl::BlockReleased);
	Input->BindAction(IA_AttackHeld, ETriggerEvent::Triggered, this, &ABrawl::AttackHeld);
	Input->BindAction(IA_AttackReleased, ETriggerEvent::Triggered, this, &ABrawl::AttackReleased);
}

void ABrawl::DirectionSelected(const FInputActionValue& ActionValue)
{
	const FVector2D InputAxisVector{ ActionValue.Get<FVector2D>() };

	const bool bMaxValueIsOnX {FMath::Abs(InputAxisVector.X) > FMath::Abs(InputAxisVector.Y)};

	if (InputAxisVector.X > 0.25 && bMaxValueIsOnX)
	{
		Cast<ABrawlerBase>(CA_Player->GetChildActor())->SelectCombatDirection(ECombatDirection::Right);
	}
	else if (InputAxisVector.X < - 0.25 && bMaxValueIsOnX)
	{
		Cast<ABrawlerBase>(CA_Player->GetChildActor())->SelectCombatDirection(ECombatDirection::Left);
	}
	else if (InputAxisVector.Y < -0.25 && !bMaxValueIsOnX)
	{
		Cast<ABrawlerBase>(CA_Player->GetChildActor())->SelectCombatDirection(ECombatDirection::Bottom);
	}
	else if (InputAxisVector.Y > 0.25 && !bMaxValueIsOnX)
	{
		Cast<ABrawlerBase>(CA_Player->GetChildActor())->SelectCombatDirection(ECombatDirection::Top);
	}
}

void ABrawl::AttackReleased()
{
	Cast<ABrawlerBase>(CA_Player->GetChildActor())->Attack();
	Attack(Player, Enemy1, Player->GetAttackData());
}

void ABrawl::Attack(ABrawlerBase* Attacker, ABrawlerBase* Defender, FAttackData AttackData)
{
	//defense from spaming attack button
	if (Attacker->CanAttack())
	{
		Attacker->Attack();
		if (Defender->IsKOd())
		{		
			Attacker->EnterFinisherCode();
		}
		else
		{
			Defender->GetHit(AttackData);		
		}
	}
}

void ABrawl::AttackHeld()
{
	Player->IncrementAttackPower();
}

void ABrawl::BlockReleased()
{
	Player->ReleaseBlock();
}

void ABrawl::BlockHeld()
{
	Player->HoldBlock();
}

void ABrawl::Reset()
{
	SetActorTransform(InitialLocation);
	Enemy1->Reset();
	Player->Reset();
	BrawlLogic->Reset();
	SetActorHiddenInGame(false);
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly{});
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}