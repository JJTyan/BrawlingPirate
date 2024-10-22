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

	//Anim montage
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Animation;

	//Tag of this montage, so it can be searched
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;

	//Tag that specifies type of hit reaction when not blocked (for combat animations)
	UPROPERTY(EditAnywhere)
	FGameplayTag HitReactionTag;

	//Tag that specifies type of hit reaction when blocked (for combat animations)
	UPROPERTY(EditAnywhere)
	FGameplayTag BlockReactionTag;

	//If true - IK will be enabled for this animation
	UPROPERTY(EditAnywhere)
	bool bUseIK {false};
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
