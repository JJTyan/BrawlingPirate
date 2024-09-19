// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrawlerBase.h"
#include "GameFramework/Pawn.h"
#include "Brawl.generated.h"

struct FInputActionValue;
class UBrawlLogic;
class USplineComponent;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class UFloatingPawnMovement;
class AMovementScheme;

UCLASS()
class MYPROJECT_API ABrawl : public APawn
{
	GENERATED_BODY()

public:
	ABrawl();

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack(ABrawlerBase* Attacker, ABrawlerBase* Defender, FAttackData AttackData);

private:
	FRotator GetOrientToMovementRotation() const;

public:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Pivot;

	//Player character
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UChildActorComponent> CA_Player;

	//Enemy Character
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UChildActorComponent> CA_Enemy1;

	//Behaviour component for AI enemies
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBrawlLogic> BrawlLogic;

	//player's camera
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UCameraComponent> FPCamera;

	//movement
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> MovementCmp;

	//Curve that defines our movement in the level
	UPROPERTY(EditAnywhere)
	TObjectPtr<AMovementScheme> MovementScheme;

private:

#pragma region INPUT BINDINGS
	void DirectionSelected(const FInputActionValue& ActionValue);
	void AttackReleased();
	void AttackHeld();
	void BlockReleased();
	void BlockHeld();
#pragma endregion
	
private:
	ABrawlerBase* Player {nullptr};
	ABrawlerBase* Enemy1 {nullptr};

#pragma region INPUTS

	UPROPERTY(EditAnywhere, category = Input)
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, category = Input)
	TObjectPtr<UInputAction> IA_SelectDirection;

	UPROPERTY(EditAnywhere, category = Input)
	TObjectPtr<UInputAction> IA_BlockHeld;

	UPROPERTY(EditAnywhere, category = Input)
	TObjectPtr<UInputAction> IA_BlockReleased;

	UPROPERTY(EditAnywhere, category = Input)
	TObjectPtr<UInputAction> IA_AttackHeld;

	UPROPERTY(EditAnywhere, category = Input)
	TObjectPtr<UInputAction> IA_AttackReleased;

#pragma endregion
};
