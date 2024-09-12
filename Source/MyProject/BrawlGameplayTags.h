// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
*
* Singleton that contains native Gameplay Tags.
* We use it to create FGameplayTags and bind them to variables with user-friendly names
*/

struct FBrawlGameplayTags
{

public:
	static const FBrawlGameplayTags& Get() { return  GameplayTags; };
	static void InitializeNativeTags();

	FGameplayTag Fast_Attack_Left;
	FGameplayTag Fast_Attack_Right;
	FGameplayTag Charged_Attack_Left;
	FGameplayTag Charged_Attack_Right;
	FGameplayTag Block_Left;
	FGameplayTag Block_Right;

private:
	static FBrawlGameplayTags GameplayTags;

};
