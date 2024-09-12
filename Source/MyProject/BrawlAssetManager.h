// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BrawlAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBrawlAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	/** Returns the current AuraAssetManager object */
	static UBrawlAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
