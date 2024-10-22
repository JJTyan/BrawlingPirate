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

	FGameplayTag Fast_Attack_Top;
	FGameplayTag Fast_Attack_Bottom;
	FGameplayTag Fast_Attack_Left;
	FGameplayTag Fast_Attack_Right;

	FGameplayTag Charged_Attack_Left;
	FGameplayTag Charged_Attack_Right;
	FGameplayTag Charged_Attack_Top;
	FGameplayTag Charged_Attack_Bottom;

	FGameplayTag Block_Top;
	FGameplayTag Block_Bottom;
	FGameplayTag Block_Left;
	FGameplayTag Block_Right;

	FGameplayTag HitReact_Block_Straight;
	FGameplayTag HitReact_Block_Left;
	FGameplayTag HitReact_Block_Right;
	FGameplayTag HitReact_Head_Straight;
	FGameplayTag HitReact_Head_Left;
	FGameplayTag HitReact_Head_Right;
	FGameplayTag HitReact_Body_Straight;
	FGameplayTag HitReact_Body_Left;
	FGameplayTag HitReact_Body_Right;

	FGameplayTag Stance_KO;

private:
	static FBrawlGameplayTags GameplayTags;
};
