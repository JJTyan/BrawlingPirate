// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrawlerBase.h"
#include "UObject/NoExportTypes.h"
#include "BrawlWidgetController.generated.h"

/**
 * A widget controller (Model-Controller-View pattern), it binds to all sources of information and directs it to the overlay widget
 */

struct FBrawlerValues
{
	float Health;
	float MaxHealth;
	float CurrentBlockValue;
	float MaxBlockValue;
	float CurrentAttackValue;
	float MaxAttackValue;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerNumericChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerDirectionChangedSignature, ECombatDirection, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerBooleanChangedSignature, bool, NewValue);

UCLASS()
class MYPROJECT_API UBrawlWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void InitializeController(ABrawlerBase* SourceCharacter, FBrawlerValues InitialValues);

	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerNumericChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerNumericChangedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerNumericChangedSignature OnBlockChanged;
	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerNumericChangedSignature OnMaxBlockChanged;
	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerNumericChangedSignature OnAttackChanged;
	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerNumericChangedSignature OnMaxAttackChanged;
	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerDirectionChangedSignature OnDirectionChanged;

	UPROPERTY(BlueprintAssignable, category = "Attributes")
	FOnControllerBooleanChangedSignature OnBlockStateChanged;

private:
	void BindToSource();

	void BroadcastInitialValues(FBrawlerValues InitialValues);

	ABrawlerBase* Brawler;
};
