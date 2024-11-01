// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrawlerBase.h"
#include "Components/ActorComponent.h"
#include "DA_Icons.h"
#include "BrawlLogic.generated.h"

DECLARE_DELEGATE(FEnemyActionSignature)

//Describes enemy action - what and when will be executed
USTRUCT(BlueprintType)
struct FEnemyAction
{
	GENERATED_BODY()

	//start time as it will be seen by player in UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SeemedStartTime;
	//end time as it will be seen by player in UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SeemedEndTime;
	//Icon for the UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	//Color of the Icon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FColor Color;

	//actual time of starting of ability
	float ActionStartTime;
	//actual time of ending the ability
	float ActionEndTime;

	//Function to be executed at ActionStartTime
	FEnemyActionSignature AcionStartDelegate;
	//Function to be executed at ActionEndTime
	FEnemyActionSignature AcionEndDelegate;
	
	ECombatDirection Direction;
};



DECLARE_MULTICAST_DELEGATE_OneParam(FEnemyActionAddedSignature, FEnemyAction)
DECLARE_MULTICAST_DELEGATE(FEnemyActionRemovedSignature)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UBrawlLogic : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBrawlLogic();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetProperties(ABrawlerBase* NewPlayer, ABrawlerBase* NewEnemy);
	void Reset();

protected:
	virtual void BeginPlay() override;

private:
	void TimerTick(float DeltaTime);
	void SetOverlay();
	void AddActionToStack();
	bool ShouldAddNewAction() const;
	void StopLogic();

	//randomly select 1 icon for each possible tag from Icons data asset
	void SetIcons();
	
public:
	FEnemyActionRemovedSignature OnReset;
	FEnemyActionRemovedSignature OnLogicStopped;
	FEnemyActionRemovedSignature OnActionRemoved;
	FEnemyActionAddedSignature OnActionAdded;

private:
	UPROPERTY(EditAnywhere, category = "Properties", meta = (AllowPrivateAccess = "true"))
	float DesiredStackLengthSeconds {10.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Properties", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBrawlOverlay> OverlayClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDA_Icons> DA_Icons;

	//Icons that will be randomly-selected from general DA_Icons for this combat.
	TMap<FGameplayTag,FIconData> IconsData;

	//Here we hold all actions we planned for the enemy NPC
	TArray<FEnemyAction> ActionStack;

	//Timer hat we use to track when action should be executed
	float Timer {0.f};

	//Flag to pause logic
	bool bStopped {false};

	ABrawlerBase* Player;
	ABrawlerBase* Enemy;
	

		
};
