// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "BrawlGameplayTags.h"
#include "Blueprint/UserWidget.h"
#include "BrawlOverlay.h"
#include "BrawlWidgetController.h"

ABrawlerBase::ABrawlerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
}

void ABrawlerBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentAttackPower = 0.f;
	CurrentBlockPower = 0.f;

	if (IsValid(OverlayClass))
	{
		SetOverlay();
	}
}

void ABrawlerBase::SetOverlay()
{
	FName WidgetName = FName("Overlay_" + GetName());
	auto OverlayWidget {CreateWidget<UBrawlOverlay>(GetWorld(), OverlayClass,WidgetName)};
	FName ControllerName = FName("OverlayController_" + GetName());
	auto WidgetController {NewObject<UBrawlWidgetController>(GetWorld(), UBrawlWidgetController::StaticClass(), ControllerName)};
	OverlayWidget->SetWidgetController(WidgetController);
	FBrawlerValues InitialValues {CurrentHealth, CurrentHealth,CurrentBlockPower, BLOCK_BASE_VALUE, CurrentAttackPower, ATTACK_MAX_VALUE};
	WidgetController->InitializeController(this, InitialValues);
	OverlayWidget->AddToViewport();
}

void ABrawlerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentAttackPower < ATTACK_BASE_VALUE)
	{
		RestoreAttackPower();
	}

	if (bHoldingBlock)
	{
		DecrementBlockPower();

		if (FMath::IsNearlyZero(CurrentBlockPower))
		{
			bHoldingBlock = false;
		}
	}

	if (!bHoldingBlock && CurrentBlockPower <= BLOCK_BASE_VALUE)
	{
		RestoreBlockPower();
	}
}

void ABrawlerBase::IncrementAttackPower()
{
	if (FMath::IsNearlyEqual(CurrentAttackPower, ATTACK_BASE_VALUE, 0.01) || CurrentAttackPower > ATTACK_BASE_VALUE)
	{
		CurrentAttackPower += ATTACK_INCREMENT_PER_SEC * GetWorld()->GetDeltaSeconds();
		CurrentAttackPower = FMath::Min(CurrentAttackPower, ATTACK_MAX_VALUE);
		OnAttackValueChanged.Broadcast(CurrentAttackPower);
	}
}

void ABrawlerBase::SelectCombatDirection(ECombatDirection NewDirection)
{
	ActiveHand = NewDirection;
	OnDirectionChanged.Broadcast(ActiveHand);
}

void ABrawlerBase::Attack()
{
	FGameplayTag AnimationTagToSearch;

	if (CurrentAttackPower > ATTACK_BASE_VALUE)
	{
		switch (ActiveHand)
		{
		case ECombatDirection::None_Max:
			[[fallthrough]];
			//break;
		case ECombatDirection::Top:
			[[fallthrough]];
			//break;
		case ECombatDirection::Bottom:
			[[fallthrough]];
			//break;
		case ECombatDirection::Left:
			AnimationTagToSearch = FBrawlGameplayTags::Get().Charged_Attack_Left;
			break;
		case ECombatDirection::Right:
			AnimationTagToSearch = FBrawlGameplayTags::Get().Charged_Attack_Right;
			break;
		
		default:
			break;
		}	
	}
	else
	{
		switch (ActiveHand)
		{
		case ECombatDirection::None_Max:
			[[fallthrough]];
			//break;
		case ECombatDirection::Top:
			[[fallthrough]];
			//break;
		case ECombatDirection::Bottom:
			[[fallthrough]];
			//break;
		case ECombatDirection::Left:
			AnimationTagToSearch = FBrawlGameplayTags::Get().Fast_Attack_Left;
			break;
		case ECombatDirection::Right:
			AnimationTagToSearch = FBrawlGameplayTags::Get().Fast_Attack_Right;
			break;
		default:
			break;
		}
	}

	Mesh->GetAnimInstance()->Montage_Play(FindAnimByTag(AnimationTagToSearch), 0.5f);
	DropAttackPower();
}

UAnimMontage* ABrawlerBase::FindAnimByTag(FGameplayTag AnimationTagToSearch)
{
	TArray<UAnimMontage*> PossibleAnimations;
	for (const FAnimData& AnimData : Animations->Animations)
	{
		if (AnimData.Tag == AnimationTagToSearch)
		{
			PossibleAnimations.Add(AnimData.Animation);
		}
	}

	return PossibleAnimations[FMath::RandRange(0, PossibleAnimations.Num() - 1)];
}

void ABrawlerBase::GetHit(FAttackData AttackData)
{
	//if not blocking at all - block 10% of CurrentBlockingPower
	//If blocking at wrong direction - block 33% of CurrentBlockingPower
	//If blocking at correct direction - block 100% of CurrentBlockingPower

	//define blocking direction. Right hand blocks enemy Left and vice versa
	ECombatDirection BlockingDirection = ActiveHand;
	if (ActiveHand == ECombatDirection::Right)
	{
		BlockingDirection = ECombatDirection::Left;
	}
	else if (ActiveHand == ECombatDirection::Left)
	{
		BlockingDirection = ECombatDirection::Right;
	}


	float DamageReduction = CurrentBlockPower;
	if (!bHoldingBlock)
	{
		DamageReduction *= 0.1f;
	}
	else if (AttackData.AttackDirection != BlockingDirection)
	{
		DamageReduction *= 0.33f;
	}
	const float RecievedDamage {AttackData.AttackPower - DamageReduction};

	CurrentHealth -= RecievedDamage;
	OnHealthChanged.Broadcast(CurrentHealth);

}

void ABrawlerBase::RestoreAttackPower()
{
	CurrentAttackPower += ATTACK_RESTORE_PER_SEC * GetWorld()->GetDeltaSeconds();
	OnAttackValueChanged.Broadcast(CurrentAttackPower);
}

void ABrawlerBase::DropAttackPower()
{
	CurrentAttackPower /= 2.f;
	OnAttackValueChanged.Broadcast(CurrentAttackPower);
}

void ABrawlerBase::HoldBlock()
{
	//broadcast only once when changing from false to true
	if (!bHoldingBlock)
	{
		OnBlockStateChanged.Broadcast(1.f);
	}

	bHoldingBlock = true;

	//If we are not playing any montage - play block animation
	if (!Mesh->GetAnimInstance()->Montage_IsPlaying(nullptr))
	{
		FGameplayTag AnimationTagToSearch;

		switch (ActiveHand)
		{
		case ECombatDirection::None_Max:
			[[fallthrough]];
			//break;
		case ECombatDirection::Top:
			[[fallthrough]];
			//break;
		case ECombatDirection::Bottom:
			[[fallthrough]];
			//break;
		case ECombatDirection::Left:
			AnimationTagToSearch = FBrawlGameplayTags::Get().Block_Left;
			break;
		case ECombatDirection::Right:
			AnimationTagToSearch = FBrawlGameplayTags::Get().Block_Right;
			break;

		default:
			break;
		}

		Mesh->GetAnimInstance()->Montage_Play(FindAnimByTag(AnimationTagToSearch), 0.5f);
	}
	else
	{
		//if we are already playing an animation - we need to check if we need to change left-right direction

		//check if current animation is blocking and get it's direction
		UAnimMontage* CurrentPlayingMontage {Mesh->GetAnimInstance()->GetCurrentActiveMontage()};

		for (const FAnimData& AnimData : Animations->Animations)
		{
			if ((AnimData.Animation == CurrentPlayingMontage) && (AnimData.Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Block"))))
			{
				if (ActiveHand == ECombatDirection::Left && AnimData.Tag == FBrawlGameplayTags::Get().Block_Right)
				{
					Mesh->GetAnimInstance()->Montage_Play(FindAnimByTag(FBrawlGameplayTags::Get().Block_Left), 0.5f);
				}
				else if (ActiveHand == ECombatDirection::Right && AnimData.Tag == FBrawlGameplayTags::Get().Block_Left)
				{
					Mesh->GetAnimInstance()->Montage_Play(FindAnimByTag(FBrawlGameplayTags::Get().Block_Right), 0.5f);
				}
			}
		}
	}
}

void ABrawlerBase::ReleaseBlock()
{
	OnBlockStateChanged.Broadcast(-1.f);
	bHoldingBlock = false;
	Mesh->GetAnimInstance()->Montage_Stop(0.5f);
}

void ABrawlerBase::DecrementBlockPower()
{
	CurrentBlockPower -= BLOCK_DECREMENT_PER_SEC * GetWorld()->GetDeltaSeconds();
	CurrentBlockPower = FMath::Max(CurrentBlockPower,0.f);
	OnBlockValueChanged.Broadcast(CurrentBlockPower);
}

void ABrawlerBase::RestoreBlockPower()
{
	CurrentBlockPower += BLOCK_RESTORE_PER_SEC * GetWorld()->GetDeltaSeconds();
	CurrentBlockPower = FMath::Min(CurrentBlockPower, BLOCK_BASE_VALUE);
	OnBlockValueChanged.Broadcast(CurrentBlockPower);
}

