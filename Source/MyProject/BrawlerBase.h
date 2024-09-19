// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DA_Animations.h"
#include "GameFramework/Actor.h"
#include "BrawlerBase.generated.h"

class UBrawlOverlay;

UENUM(BlueprintType)
enum class ECombatDirection : uint8
{
	Top,
	Bottom,
	Left,
	Right,
	None_Max UMETA(Hidden)
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNumericAttributeChangedSignature, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDirectionAttributeChangedSignature, ECombatDirection);

struct FAttackData
{
	float AttackPower;
	ECombatDirection AttackDirection;
};

UCLASS()
class MYPROJECT_API ABrawlerBase : public AActor
{
	friend class UBrawlLogic;

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrawlerBase();

	FAttackData GetAttackData() const {return FAttackData(CurrentAttackPower,ActiveHand); }

	//returns true if attack is possible
	bool CanAttack() const {return CurrentAttackPower >= ATTACK_THRESHOLD_VALUE;}


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//	Increase current attack power by IncrementValue, if already restored to base value
	void IncrementAttackPower();

	//Set Attack Direction
	void SelectCombatDirection(ECombatDirection NewDirection);

	//Perform attack
	void Attack();

	UAnimMontage* FindAnimByTag(FGameplayTag AnimationTagToSearch);


	//Recieve an attack 
	void GetHit(FAttackData AttackData);

	//Block is held and ready to deflect incoming attack
	void HoldBlock();

	//Block button is released
	void ReleaseBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	//	Increase current attack power by Restore Value until it reaches base value
	void RestoreAttackPower();

	// Drop attack power after attacking
	void DropAttackPower();

	//Block power is going down each tick
	void DecrementBlockPower();

	//Block power is going up each tick
	void RestoreBlockPower();

	//Create an overlay and it's controller to show this character data 
	virtual void SetOverlay();


public:
	//broadcasts current health
	FOnNumericAttributeChangedSignature OnHealthChanged;
	//broadcasts current attack power
	FOnNumericAttributeChangedSignature OnAttackValueChanged;
	//broadcasts current block
	FOnNumericAttributeChangedSignature OnBlockValueChanged;
	//Is block held or not. Kinda bool, but let it be float
	FOnNumericAttributeChangedSignature OnBlockStateChanged;
	//broadcasts current direction
	FOnDirectionAttributeChangedSignature OnDirectionChanged;


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	//Direction that will be used to attack
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatDirection ActiveHand {ECombatDirection::None_Max};

	//Amount of damage that can be resisted now
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth {50.f};

	//Amount of damage that can be resisted now
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CurrentBlockPower;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//Amount of damage that can be dealt now
	float CurrentAttackPower;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//Amount of damage that can be dealt now
	bool bHoldingBlock {false};


# pragma region PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBrawlOverlay> OverlayClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDA_Animations> Animations;

	//Min attack power needed to perform an attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	float ATTACK_THRESHOLD_VALUE{ 2.f };

	//Amount of Attack increment if lower then base value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	float ATTACK_RESTORE_PER_SEC {1.f};

	//Amount of Attack increment if lower then resting point
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	float ATTACK_BASE_VALUE{ 10.f };

	//Max amount of Attack we can reach when charging
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	float ATTACK_MAX_VALUE{ 30.f };

	//Amount of Attack increment if corresponding butåon is held
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	float ATTACK_INCREMENT_PER_SEC{ 2.5f };

	//Amount of Block increment
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	float BLOCK_RESTORE_PER_SEC{ 1.5f };

	//Amount of Block decrement when corresponding buttåon is held
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	float BLOCK_DECREMENT_PER_SEC{ 3.f };

	//Base amount of block. This is also max amount
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	float BLOCK_BASE_VALUE{ 15.f };



# pragma endregion

	//Direction of current blocking animation.
	ECombatDirection BlockingAnimDirection;
};
