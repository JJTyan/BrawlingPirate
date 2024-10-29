// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlGameplayTags.h"
#include "GameplayTagsManager.h"

FBrawlGameplayTags FBrawlGameplayTags::GameplayTags;

void FBrawlGameplayTags::InitializeNativeTags()
{
	GameplayTags.Fast_Attack_Top = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Fast.Top"), FString(TEXT("Top hand fast attack")));
	GameplayTags.Fast_Attack_Bottom = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Fast.Bottom"), FString(TEXT("Bottom hand fast attack")));
	GameplayTags.Fast_Attack_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Fast.Left"), FString(TEXT("Left hand fast attack")));
	GameplayTags.Fast_Attack_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Fast.Right"), FString(TEXT("Right hand fast attack")));
	
	GameplayTags.Charged_Attack_Top = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Charged.Top"), FString(TEXT("Top hand charged attack")));
	GameplayTags.Charged_Attack_Bottom = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Charged.Bottom"), FString(TEXT("Bottom hand charged attack")));
	GameplayTags.Charged_Attack_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Charged.Left"), FString(TEXT("Left hand charged attack")));
	GameplayTags.Charged_Attack_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attack.Charged.Right"), FString(TEXT("Right hand charged attack")));
	
	GameplayTags.Block_Top = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Block.Top"), FString(TEXT("Top hand block")));
	GameplayTags.Block_Bottom = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Block.Bottom"), FString(TEXT("Bottom hand block")));
	GameplayTags.Block_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Block.Left"), FString(TEXT("Left hand block")));
	GameplayTags.Block_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Block.Right"), FString(TEXT("Right hand block")));
	
	GameplayTags.HitReact_BlockFull_Straight = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.Block.Straight"), FString(TEXT("HitReact Block Straight")));
	GameplayTags.HitReact_BlockFull_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.Block.Left"), FString(TEXT("HitReact Block Left")));
	GameplayTags.HitReact_BlockFull_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.Block.Right"), FString(TEXT("HitReact Block Right")));
	GameplayTags.HitReact_BlockPartial_Straight = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.BlockPartial.Straight"), FString(TEXT("HitReact BlockPartial Straight")));
	GameplayTags.HitReact_BlockPartial_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.BlockPartial.Left"), FString(TEXT("HitReact BlockPartial Left")));
	GameplayTags.HitReact_BlockPartial_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.BlockPartial.Right"), FString(TEXT("HitReact BlockPartial Right")));


	GameplayTags.HitReact_Head_Straight = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.Head.Straight"), FString(TEXT("HitReact Head Straight")));
	GameplayTags.HitReact_Head_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.Head.Left"), FString(TEXT("HitReact Head Left")));
	GameplayTags.HitReact_Head_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.Head.Right"), FString(TEXT("HitReact Head Right")));
	GameplayTags.HitReact_Body_Straight = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.Body.Straight"), FString(TEXT("HitReact Body Straight")));
	GameplayTags.HitReact_Body_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.Body.Left"), FString(TEXT("HitReact Body Left")));
	GameplayTags.HitReact_Body_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("HitReact.Body.Right"), FString(TEXT("HitReact Body Right")));

	GameplayTags.Stance_KO = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Stance.KO"), FString(TEXT("Knock out")));
}
