// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlGameplayTags.h"
#include "GameplayTagsManager.h"

FBrawlGameplayTags FBrawlGameplayTags::GameplayTags;

void FBrawlGameplayTags::InitializeNativeTags()
{
	GameplayTags.Fast_Attack_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Fast.Left"), FString(TEXT("Left hand fast attack")));
	GameplayTags.Fast_Attack_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Fast.Right"), FString(TEXT("Right hand fast attack")));;
	GameplayTags.Charged_Attack_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Charged.Left"), FString(TEXT("Left hand charged attack")));;
	GameplayTags.Charged_Attack_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Charged.Right"), FString(TEXT("Right hand charged attack")));;
	GameplayTags.Block_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Block.Left"), FString(TEXT("Left hand block")));;
	GameplayTags.Block_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Block.Right"), FString(TEXT("Right hand block")));;
}
