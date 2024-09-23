// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionsWidgetController.h"

void UActionsWidgetController::InitializeController(UBrawlLogic* BrawlLogicSource)
{
	BrawlLogic = BrawlLogicSource;
	BindToSource();
}

void UActionsWidgetController::BindToSource()
{
	BrawlLogic->OnActionAdded.AddLambda(
		[this](FEnemyAction AddedAction)
		{
			OnActionAdded.Broadcast(AddedAction);
		});

	BrawlLogic->OnActionRemoved.AddLambda(
		[this]()
		{
			OnActionRemoved.Broadcast();
		});

	BrawlLogic->OnLogicStopped.AddLambda(
		[this]()
		{
			OnLogicStopped.Broadcast();
		});
}
