// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DA_Animations.generated.h"

/**
 * 
 */

 USTRUCT()
struct FAnimData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Animation;

	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
};

UCLASS()
class MYPROJECT_API UDA_Animations : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	//List of animations
	UPROPERTY(EditAnywhere)
	TArray<FAnimData>Animations;
	
};
