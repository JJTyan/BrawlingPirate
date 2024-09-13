// Fill out your copyright notice in the Description page of Project Settings.

#include "Brawl.h"
#include "BrawlLogic.h"
#include "Components/SplineComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"

ABrawl::ABrawl()
{
	PrimaryActorTick.bCanEverTick = true;

	CA_Player = CreateDefaultSubobject<UChildActorComponent>(TEXT("Player"));
	CA_Enemy1 = CreateDefaultSubobject<UChildActorComponent>(TEXT("Enemy1"));
	BrawlLogic = CreateDefaultSubobject<UBrawlLogic>(TEXT("Brawl Logic Component"));
	RoutComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Rout Component"));
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	FPCamera->SetupAttachment(CA_Player);
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

	Player = Cast<ABrawlerBase>(CA_Player->GetChildActor());
	Enemy1 = Cast<ABrawlerBase>(CA_Enemy1->GetChildActor());
	BrawlLogic->SetProperties(Player, Enemy1);
}

void ABrawl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void ABrawl::Attack(ABrawlerBase* Attacker, ABrawlerBase* Defender, FAttackData AttackData)
{
	//defence from spamming attack button
	if (Attacker->CanAttack())
	{
		Attacker->Attack();
		Defender->GetHit(AttackData);
	}
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

