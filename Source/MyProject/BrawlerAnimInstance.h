// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BrawlerBase.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BrawlerAnimInstance.generated.h"

/**
 * 
 */

 class ABrawlerBase;

UCLASS()
class MYPROJECT_API UBrawlerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	void InitializeAnimation(ABrawlerBase* Brawler);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayHitReacion(FGameplayTag Tag);

	//Location - world space target for IK
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void StartLeftHandIK(FVector Location);

	//Location - world space target for IK
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StartRightHandIK(FVector Location);

	//Location - world space target for IK
	UFUNCTION(BlueprintCallable)
	void SetIKTargetLocation(FVector Location) { FistIKTarget = Location;};

private:
	UFUNCTION()
	void GetStanceDirection(ECombatDirection NewDirection);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ABrawlerBase* Owner {nullptr};
	
	UPROPERTY(BlueprintReadOnly, category = Stance, meta = (AllowPrivateAccess = "true"))
	float LeftRightTarget {0.f};

	UPROPERTY(BlueprintReadOnly, category = Stance, meta = (AllowPrivateAccess = "true"))
	float TopBottomTarget {0.f};

	UPROPERTY(BlueprintReadWrite, category = Stance, meta = (AllowPrivateAccess = "true"))
	float LeftRightCurrent {0.f};

	UPROPERTY(BlueprintReadWrite, category = Stance, meta = (AllowPrivateAccess = "true"))
	float TopBottomCurrent {0.f};

	//When hand is glued to body by IK this is how much it is allowed to move from start position to prevent stretching too far
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, category = Properties, meta = (AllowPrivateAccess = "true"))
	float MAX_HAND_MOVEMENT_WHILE_IK {50.f};

	//Desired location of IK effector in component space
	UPROPERTY(BlueprintReadOnly, Transient, meta = (AllowPrivateAccess = "true"))
	FVector FistIKTarget;

	//Location of IK effector when it was just triggered
	UPROPERTY(BlueprintReadWrite, Transient, meta = (AllowPrivateAccess = "true"))
	FVector IKStart;
	
};
