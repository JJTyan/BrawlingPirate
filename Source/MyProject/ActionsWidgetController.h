// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BrawlLogic.h"
#include "ActionsWidgetController.generated.h"

/**
 * A widget controller (Model-Controller-View pattern), it binds to all sources of information and directs it to the widget
 */

 class UBrawlLogic;

 DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerActionAddedSignature, FEnemyAction, AddedAction);
 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnControllerActionRemovedSignature);
 

UCLASS(BlueprintType)
class MYPROJECT_API UActionsWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void InitializeController(UBrawlLogic* BrawlLogicSource);

	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerActionAddedSignature OnActionAdded;

	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerActionRemovedSignature OnActionRemoved;

	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerActionRemovedSignature OnLogicStopped;

private:
	void BindToSource();
	UBrawlLogic* BrawlLogic;
	
};
