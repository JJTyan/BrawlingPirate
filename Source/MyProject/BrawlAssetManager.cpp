// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlAssetManager.h"
#include "BrawlGameplayTags.h"

UBrawlAssetManager& UBrawlAssetManager::Get()
{
	check(GEngine);

	UBrawlAssetManager* Singleton = Cast<UBrawlAssetManager>(GEngine->AssetManager);

	if (Singleton)
	{
		return *Singleton;
	}
	else
	{
		return *Cast<UBrawlAssetManager>(NewObject<UBrawlAssetManager>()); // never calls this
	}
}

void UBrawlAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FBrawlGameplayTags::InitializeNativeTags();
}