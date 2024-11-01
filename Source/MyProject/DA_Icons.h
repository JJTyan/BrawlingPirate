// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DA_Icons.generated.h"

/**
 * This is the storage of all icons we can use in enemy's action stack
 */

 USTRUCT()
struct FIconData
{
	 GENERATED_BODY()

	//Action tag for this icon
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag {FGameplayTag()};

	//Color of the Icon
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> IconAsset;

	//Color of the Icon
	UPROPERTY(EditAnywhere)
	FColor IconColor {FColor::White};

};

UCLASS()
class MYPROJECT_API UDA_Icons : public UDataAsset
{
	GENERATED_BODY()

public:
	//List of animations
	UPROPERTY(EditAnywhere)
	TArray<FIconData>Icons;

	FIconData GetRandomDataByTag(FGameplayTag Tag);
	
};
