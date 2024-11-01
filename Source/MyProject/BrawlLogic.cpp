// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlLogic.h"
#include "Blueprint/UserWidget.h"
#include "BrawlOverlay.h"
#include "ActionsWidgetController.h"
#include "BrawlGameplayTags.h"

UBrawlLogic::UBrawlLogic()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void UBrawlLogic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TimerTick(DeltaTime);
}

void UBrawlLogic::SetProperties(ABrawlerBase* NewPlayer, ABrawlerBase* NewEnemy)
{
	Player = NewPlayer;
	Enemy = NewEnemy;
	Enemy->OnKOd.AddUObject(this,&UBrawlLogic::StopLogic);
}

void UBrawlLogic::Reset()
{
	Timer = 0.f;
	bStopped = false;
	OnReset.Broadcast();
}

void UBrawlLogic::StopLogic()
{
	bStopped = true;
	ActionStack.Empty();
	OnLogicStopped.Broadcast();
}

void UBrawlLogic::BeginPlay()
{
	Super::BeginPlay();
	SetOverlay();
	SetIcons();
}

void UBrawlLogic::SetOverlay()
{
	auto OverlayWidget{ CreateWidget<UBrawlOverlay>(GetWorld(), OverlayClass) };
	auto WidgetController{ NewObject<UActionsWidgetController>(GetWorld(), UActionsWidgetController::StaticClass()) };
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->InitializeController(this);
	OverlayWidget->AddToViewport();
}

void UBrawlLogic::SetIcons()
{
	TArray < FGameplayTag> Tags
	{
		FBrawlGameplayTags::Get().Block_Bottom,
		FBrawlGameplayTags::Get().Block_Top,
		FBrawlGameplayTags::Get().Block_Left,
		FBrawlGameplayTags::Get().Block_Right,

		FBrawlGameplayTags::Get().Charged_Attack_Bottom,
		FBrawlGameplayTags::Get().Charged_Attack_Top,
		FBrawlGameplayTags::Get().Charged_Attack_Left,
		FBrawlGameplayTags::Get().Charged_Attack_Right,

		FBrawlGameplayTags::Get().Fast_Attack_Bottom,
		FBrawlGameplayTags::Get().Fast_Attack_Top,
		FBrawlGameplayTags::Get().Fast_Attack_Left,
		FBrawlGameplayTags::Get().Fast_Attack_Right
	};

	for (const FGameplayTag& Tag : Tags)
	{
		IconsData.Add(Tag, DA_Icons->GetRandomDataByTag(Tag));
	}
}

void UBrawlLogic::TimerTick(float DeltaTime)
{
	if (!(Player && Enemy) || bStopped)
	{
		return;
	}

	if (ShouldAddNewAction())
	{
		AddActionToStack();
	}

	if (FMath::IsNearlyEqual(Timer, ActionStack[0].ActionStartTime,DeltaTime))
	{
		Enemy->SelectCombatDirection(ActionStack[0].Direction);
		ActionStack[0].AcionStartDelegate.ExecuteIfBound();
		ActionStack[0].AcionStartDelegate = nullptr;
	}
	else if (FMath::IsNearlyEqual(Timer, ActionStack[0].ActionEndTime, DeltaTime))
	{
		ActionStack[0].AcionEndDelegate.ExecuteIfBound();
		ActionStack[0].AcionEndDelegate = nullptr;
	}
	else if (Timer >= ActionStack[0].SeemedEndTime)
	{
		ActionStack.RemoveAt(0);
		OnActionRemoved.Broadcast();
	}

	Timer += DeltaTime;
}

bool UBrawlLogic::ShouldAddNewAction() const
{
	if (ActionStack.Num() == 0)
	{
		return true;
	}
	const float CurrenStackLength{ ActionStack.Last().SeemedEndTime - ActionStack[0].SeemedStartTime };
	return  CurrenStackLength < DesiredStackLengthSeconds;
}

void UBrawlLogic::AddActionToStack()
{
	//For testing all enemy actions will be blocks with toggling directions

	FEnemyAction NewAction {};
	NewAction.SeemedStartTime = ActionStack.Num() == 0 ? 3.f : ActionStack.Last().SeemedEndTime + 3.f;
	NewAction.ActionStartTime = NewAction.SeemedStartTime + 0.5f;
	NewAction.ActionEndTime = NewAction.ActionStartTime + 2.f;
	NewAction.SeemedEndTime = NewAction.ActionEndTime + 0.5f;

	if (ActionStack.Num() == 0)
	{
		NewAction.Direction = ECombatDirection::Left;
		NewAction.Icon = IconsData[FBrawlGameplayTags::Get().Block_Left].IconAsset;
	}
	else if (ActionStack.Last().Direction == ECombatDirection::Left)
	{
		NewAction.Direction = ECombatDirection::Right;
		NewAction.Icon = IconsData[FBrawlGameplayTags::Get().Block_Right].IconAsset;
	}
	else if (ActionStack.Last().Direction == ECombatDirection::Right)
	{
		NewAction.Direction = ECombatDirection::Top;
		NewAction.Icon = IconsData[FBrawlGameplayTags::Get().Block_Top].IconAsset;
	}
	else if (ActionStack.Last().Direction == ECombatDirection::Top)
	{
		NewAction.Direction = ECombatDirection::Bottom;
		NewAction.Icon = IconsData[FBrawlGameplayTags::Get().Block_Bottom].IconAsset;
	}
	else
	{
		NewAction.Direction = ECombatDirection::Left;
		NewAction.Icon = IconsData[FBrawlGameplayTags::Get().Block_Left].IconAsset;
	}

	NewAction.AcionStartDelegate.BindUObject(Enemy,&ABrawlerBase::HoldBlock);
	NewAction.AcionEndDelegate.BindUObject(Enemy, &ABrawlerBase::ReleaseBlock);

	ActionStack.Add(NewAction);

	OnActionAdded.Broadcast(NewAction);

}

