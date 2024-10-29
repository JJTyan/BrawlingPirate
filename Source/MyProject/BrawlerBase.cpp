// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerBase.h"
#include "Brawl.h"
#include "BrawlerAnimInstance.h"
#include "Finisher.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "BrawlGameplayTags.h"
#include "Blueprint/UserWidget.h"
#include "BrawlOverlay.h"
#include "BrawlWidgetController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

ABrawlerBase::ABrawlerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	//Collision that deal attack
	RightFistCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Right Fist"));
	RightFistCollision->SetupAttachment(Mesh,"hand_r");
	LeftFistCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Left Fist"));
	LeftFistCollision->SetupAttachment(Mesh, "hand_l");

	//Collision that resieve attack
	HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Head Collision"));
	HeadCollision->SetupAttachment(Mesh, "head");
	BodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Body Collision"));
	BodyCollision->SetupAttachment(Mesh, "spine_02");

	IKTargetComponent = CreateDefaultSubobject<USceneComponent>(TEXT("IK Target Component"));
	IKTargetComponent->SetupAttachment(Mesh);
}

void ABrawlerBase::SetIKTarget(FVector Location, FName Socket)
{
	FAttachmentTransformRules TransformRules { EAttachmentRule::KeepRelative,true};
	IKTargetComponent->AttachToComponent(Mesh, TransformRules, Socket);
	IKTargetComponent->SetWorldLocation(Location);
}

FVector ABrawlerBase::GetIKTargetLocation() const
{
	return IKTargetComponent->GetComponentLocation();
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

	CreateSkinDMI();
	Cast<UBrawlerAnimInstance>(Mesh->GetAnimInstance())->InitializeAnimation(this);
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

void ABrawlerBase::EnterFinisherCode()
{
	//TODO For now there is no code to check, so we assume that anything will lead to the finisher
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
		{
			SpawnFinisher();
			OnFinisherUsed.Broadcast();
		}, 0.5f, false);
}

void ABrawlerBase::SpawnFinisher()
{
	UClass* FinisherToSpawn	{Finishers.Array()[FMath::RandRange(0, Finishers.Num() - 1)]};
	FTransform SpawnTransform { FTransform(FVector::ZeroVector) };
	AFinisher* Finisher {GetWorld()->SpawnActorDeferred<AFinisher>(FinisherToSpawn,SpawnTransform)};
	Finisher->SetBrawl(Cast<ABrawl>(GetParentActor()));
	UGameplayStatics::FinishSpawningActor(Finisher, SpawnTransform);
}

void ABrawlerBase::CreateSkinDMI()
{
	TArray<UMaterialInterface*> Materials{ Mesh->GetMaterials() };
	UMaterialInstanceDynamic* DynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, Materials[0]);
	Mesh->SetMaterial(0, DynamicMaterial);
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

void ABrawlerBase::RestoreAttackPower()
{
	CurrentAttackPower += ATTACK_RESTORE_PER_SEC * GetWorld()->GetDeltaSeconds();
	OnAttackValueChanged.Broadcast(CurrentAttackPower);
}

void ABrawlerBase::DecrementBlockPower()
{
	CurrentBlockPower -= BLOCK_DECREMENT_PER_SEC * GetWorld()->GetDeltaSeconds();
	CurrentBlockPower = FMath::Max(CurrentBlockPower, 0.f);
	OnBlockValueChanged.Broadcast(CurrentBlockPower);
}

void ABrawlerBase::RestoreBlockPower()
{
	CurrentBlockPower += BLOCK_RESTORE_PER_SEC * GetWorld()->GetDeltaSeconds();
	CurrentBlockPower = FMath::Min(CurrentBlockPower, BLOCK_BASE_VALUE);
	OnBlockValueChanged.Broadcast(CurrentBlockPower);
}

void ABrawlerBase::IncrementAttackPower()
{
	if (FMath::IsNearlyEqual(CurrentAttackPower, ATTACK_BASE_VALUE, 0.01) || CurrentAttackPower > ATTACK_BASE_VALUE)
	{
		CurrentAttackPower += ATTACK_INCREMENT_PER_SEC * GetWorld()->GetDeltaSeconds();
		CurrentAttackPower = FMath::Min(CurrentAttackPower, ATTACK_MAX_VALUE);
		OnAttackValueChanged.Broadcast(CurrentAttackPower);
		const float EmissiveAmount { FMath::GetMappedRangeValueClamped(TRange<float>(ATTACK_BASE_VALUE, ATTACK_MAX_VALUE), TRange<float>(0.f, 1.f),CurrentAttackPower) };
		Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(0))->SetScalarParameterValue(FName("EmissiveHandsStrength"), EmissiveAmount);
	}
}

void ABrawlerBase::SelectCombatDirection(ECombatDirection NewDirection)
{
	ActiveHand = NewDirection;
	OnDirectionChanged.Broadcast(ActiveHand);
}

void ABrawlerBase::Attack()
{
	FAnimData AnimData;
	PlayAttackMontage(AnimData);
	bUseHandIK = AnimData.bUseIK;

	OutAttackData.AttackPower = CurrentAttackPower;
	OutAttackData.AttackDirection = ActiveHand;
	OutAttackData.AnimData = AnimData;
	
	//remove "gloving fists effect"
	Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(0))->SetScalarParameterValue(FName("EmissiveHandsStrength"), 0);	
	
	DropAttackPower();
}

void ABrawlerBase::PlayAttackMontage(FAnimData& AnimData)
{
	FGameplayTag AnimationTagToSearch;

	if (CurrentAttackPower > ATTACK_BASE_VALUE * 1.5f)		//condition to play "charged" animation
	{
		switch (ActiveHand)
		{
		case ECombatDirection::None_Max:
			[[fallthrough]];
			//break;
		case ECombatDirection::Top:
			AnimationTagToSearch = FBrawlGameplayTags::Get().Charged_Attack_Top;
			break;
		case ECombatDirection::Bottom:
			AnimationTagToSearch = FBrawlGameplayTags::Get().Charged_Attack_Bottom;
			break;
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
			AnimationTagToSearch = FBrawlGameplayTags::Get().Fast_Attack_Top;
			break;
		case ECombatDirection::Bottom:
			AnimationTagToSearch = FBrawlGameplayTags::Get().Fast_Attack_Bottom;
			break;
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
	AnimData = FindAnimDataByTag(AnimationTagToSearch);
	Mesh->GetAnimInstance()->Montage_Play(AnimData.Animation, 0.5f);
}

FAnimData ABrawlerBase::FindAnimDataByTag(FGameplayTag AnimationTagToSearch)
{
	TArray<FAnimData> PossibleAnimations;
	for (const FAnimData& AnimData : Animations->Animations)
	{
		if (AnimData.Tag == AnimationTagToSearch)
		{
			PossibleAnimations.Add(AnimData);
		}
	}

	//checkf(PossibleAnimations.Num() > 0, TEXT("Anim not found by tag %s - this is not allowed"), *AnimationTagToSearch.ToString());
	return PossibleAnimations[FMath::RandRange(0, PossibleAnimations.Num() - 1)];
}

void ABrawlerBase::DropAttackPower()
{
	CurrentAttackPower /= 2.f;
	OnAttackValueChanged.Broadcast(CurrentAttackPower);
}

void ABrawlerBase::GetHit(FAttackData AttackData)
{
	HitData = AttackData;
	BlockType = GetBlockType(AttackData);
	const float RecievedDamage {AttackData.AttackPower - CalcDamageReduction()};

	CurrentHealth -= RecievedDamage;
	if (CurrentHealth <= 0)
	{
		bKO = true;
		OnKOd.Broadcast();

		Mesh->GetAnimInstance()->Montage_Play(FindAnimDataByTag(FBrawlGameplayTags::Get().Stance_KO).Animation, 0.5f);	
	}

	Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(0))->SetScalarParameterValue(FName("DamageMask"), 1 - CurrentHealth / MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);

	//Try to play hit reaction. This can happen if punch animation didn't trigger an overlap event
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,this, &ABrawlerBase::PlayHitReaction, 0.25f, false);
}

EBlockType ABrawlerBase::GetBlockType(const FAttackData& AttackData) const
{
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
		return EBlockType::None;
	}
	else if (AttackData.AttackDirection != BlockingDirection)
	{
		return EBlockType::PartialBlock;
	}

	return EBlockType::FullBlock;
}

float ABrawlerBase::CalcDamageReduction() const
{
	//if not blocking at all - block 10% of CurrentBlockingPower
	//If blocking at wrong direction - block 33% of CurrentBlockingPower
	//If blocking at correct direction - block 100% of CurrentBlockingPower

	float DamageReduction = CurrentBlockPower;
	if (BlockType == EBlockType::None)
	{
		DamageReduction *= 0.1f;
	}
	else if (BlockType == EBlockType::PartialBlock)
	{
		DamageReduction *= 0.33f;
	}

	return DamageReduction;
}

void ABrawlerBase::PlayHitReaction()
{
	if (!bPlayingHitReaction)
	{
		bPlayingHitReaction = true;

		FGameplayTag HitReactionTag;
		
		switch (BlockType)
		{
		case EBlockType::None:
			HitReactionTag = HitData.AnimData.HitReactionTag;
			break;
		case EBlockType::PartialBlock:
			HitReactionTag = HitReactionTag = HitData.AnimData.BlockPartialReactionTag;
			break;
		case EBlockType::FullBlock:
			HitReactionTag = HitReactionTag = HitData.AnimData.BlockFullReactionTag;
			break;
		case EBlockType::None_Max:
			[[fallthrough]];
		default:
			checkNoEntry();
			break;
		}
		
		Cast<UBrawlerAnimInstance>(Mesh->GetAnimInstance())->PlayHitReacion(HitReactionTag);

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]
			{
				bPlayingHitReaction = false;
			});

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5f, false);
	}
}

void ABrawlerBase::HoldBlock()
{
	//broadcast only once when changing from false to true
	if (!bHoldingBlock)
	{
		OnBlockStateChanged.Broadcast(1.f);
	}

	bHoldingBlock = true;

	FGameplayTag AnimationTagToSearch;

	switch (ActiveHand)
	{
	case ECombatDirection::None_Max:
		[[fallthrough]];
		//break;
	case ECombatDirection::Top:
		AnimationTagToSearch = FBrawlGameplayTags::Get().Block_Top;
		break;
	case ECombatDirection::Bottom:
		AnimationTagToSearch = FBrawlGameplayTags::Get().Block_Bottom;
		break;
	case ECombatDirection::Left:
		AnimationTagToSearch = FBrawlGameplayTags::Get().Block_Left;
		break;
	case ECombatDirection::Right:
		AnimationTagToSearch = FBrawlGameplayTags::Get().Block_Right;
		break;

	default:
		break;
	}	

	//If we are not playing any montage - play corresponding block animation
	if (!Mesh->GetAnimInstance()->Montage_IsPlaying(nullptr))
	{	
		Mesh->GetAnimInstance()->Montage_Play(FindAnimDataByTag(AnimationTagToSearch).Animation, 0.5f);	
	}
	else
	{
		//if we are already playing an animation - we need to check if we need to change direction

		//check if current animation is blocking and get it's direction
		UAnimMontage* CurrentPlayingMontage {Mesh->GetAnimInstance()->GetCurrentActiveMontage()};

		for (const FAnimData& AnimData : Animations->Animations)
		{
			if ((AnimData.Animation == CurrentPlayingMontage) && (AnimData.Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Block"))))
			{
				if (!AnimData.Tag.MatchesTag(AnimationTagToSearch))
				{				
					Mesh->GetAnimInstance()->Montage_Play(FindAnimDataByTag(AnimationTagToSearch).Animation, 0.5f);				
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

void ABrawlerBase::Reset()
{
	bDead = false;
	bKO = false;
	Mesh->GetAnimInstance()->Montage_Stop(0.5f);
	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(MaxHealth);
	Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(0))->SetScalarParameterValue(FName("DamageMask"), 0.f);
	SetHidden(false);
	OnReset.Broadcast();
}

void ABrawlerBase::AttachHand(bool bRightFist, FName Socket, const FVector& WorldLocation, ABrawlerBase* IKSourceActor)
{
	//TODO Logically this doesn't belong here.
	//Reset to initial state 
	bUseHandIK = false;

	//Activate hand IK in animBP
	UBrawlerAnimInstance* AnimInstance{ CastChecked<UBrawlerAnimInstance>(Mesh->GetAnimInstance()) };
	const FVector ContactLocationLS { UKismetMathLibrary::InverseTransformLocation(Mesh->GetComponentTransform(),WorldLocation)};
	if (bRightFist)
	{
		AnimInstance->StartRightHandIK(ContactLocationLS);
	}
	else
	{
		AnimInstance->StartLeftHandIK(ContactLocationLS);
	}
	
	//Move IK target component to contact location. Attach to another bone if needed (i.e. head, body, hand)
	IKSourceActor->SetIKTarget(WorldLocation, Socket);

	//feed IK location to animBP for 1 second-
	FTimerDelegate ProvideIKLocationDelegate;
	ProvideIKLocationDelegate.BindLambda([this, AnimInstance, IKSourceActor]
			{
				const FVector ContactLocationLS{ UKismetMathLibrary::InverseTransformLocation(Mesh->GetComponentTransform(),IKSourceActor->GetIKTargetLocation()) };
				AnimInstance->SetIKTargetLocation(ContactLocationLS);
				DrawDebugSphere(GetWorld(), IKSourceActor->GetIKTargetLocation(),2.5f,8,FColor::Red,false,0.1f);
			});
	GetWorldTimerManager().SetTimer(ProvideIKLocationHandle, ProvideIKLocationDelegate, GetWorld()->GetDeltaSeconds(), true);

	FTimerDelegate StopProvidingLocationDelegate;
	StopProvidingLocationDelegate.BindLambda([this]{GetWorldTimerManager().ClearTimer(ProvideIKLocationHandle);});
	FTimerHandle StopProvidingLocationHandle;
	GetWorldTimerManager().SetTimer(StopProvidingLocationHandle, StopProvidingLocationDelegate,1.f,false);

}