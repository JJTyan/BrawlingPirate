// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrawlerBase.h"
#include "Components/ActorComponent.h"
#include "BrawlLogic.generated.h"

DECLARE_DELEGATE(FEnemyActionSignature)

//Describes enemy action - what and when will be executed
struct FEnemyAction
{
	//start time as it will be seen by player in UI
	float SeemedStartTime;
	//actual time of starting of ability
	float ActionStartTime;
	//actual time of ending the ability
	float ActionEndTime;
	//end time as it will be seen by player in UI
	float SeemedEndTime;
	//Icon for the UI
	UTexture2D* Icon;
	//Function to be executed at ActionStartTime
	FEnemyActionSignature AcionStartDelegate;
	//Function to be executed at ActionEndTime
	FEnemyActionSignature AcionEndDelegate;
	
	ECombatDirection Direction;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UBrawlLogic : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBrawlLogic();
	void SetProperties(ABrawlerBase* NewPlayer, ABrawlerBase* NewEnemy) {Player = NewPlayer; Enemy = NewEnemy;};

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, category = "Properties", meta = (AllowPrivateAccess = "true"))
	float DesiredStackLengthSeconds {10.f};

	//Here we hold all actions we planned for the enemy NPC
	TArray<FEnemyAction> ActionStack;

	//Timer hat we use to track when action should be executed
	float Timer {0.f};

private:
	void TimerTick(float DeltaTime);
	void AddActionToStack();

	bool ShouldAddNewAction() const;

	ABrawlerBase* Player;
	ABrawlerBase* Enemy;

		
};
