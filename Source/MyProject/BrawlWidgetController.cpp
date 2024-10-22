// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlWidgetController.h"

void UBrawlWidgetController::InitializeController(ABrawlerBase* SourceCharacter, FBrawlerValues InitialValues)
{
	Brawler = SourceCharacter;
	BindToSource();
	BroadcastInitialValues(InitialValues);
}

void UBrawlWidgetController::BindToSource()
{
	Brawler->OnHealthChanged.AddLambda(
		[this](float Value)
		{
			OnHealthChanged.Broadcast(Value);
		});

	Brawler->OnBlockValueChanged.AddLambda(
		[this](float Value)
		{
			OnBlockChanged.Broadcast(Value);
		});

	Brawler->OnAttackValueChanged.AddLambda(
		[this](float Value)
		{
			OnAttackChanged.Broadcast(Value);
		});

	Brawler->OnDirectionChanged.AddLambda(
		[this](ECombatDirection Value)
		{
			OnDirectionChanged.Broadcast(Value);
		});

	Brawler->OnBlockStateChanged.AddLambda(
		[this](float Value)
		{
			OnBlockStateChanged.Broadcast(Value > 0);
		});
	Brawler->OnKOd.AddLambda(
		[this]()
		{
			OnKOd.Broadcast();
		});
	Brawler->OnFinisherUsed.AddLambda(
		[this]()
		{
			OnFinisherUsed.Broadcast();
		});
	Brawler->OnReset.AddLambda(
		[this]()
		{
			OnReset.Broadcast();
		});
}

void UBrawlWidgetController::BroadcastInitialValues(FBrawlerValues InitialValues)
{
	OnMaxHealthChanged.Broadcast(InitialValues.MaxHealth);
	OnHealthChanged.Broadcast(InitialValues.Health);
	OnMaxBlockChanged.Broadcast(InitialValues.MaxBlockValue);
	OnBlockChanged.Broadcast(InitialValues.CurrentBlockValue);
	OnMaxAttackChanged.Broadcast(InitialValues.MaxAttackValue);
	OnAttackChanged.Broadcast(InitialValues.CurrentAttackValue);
}


