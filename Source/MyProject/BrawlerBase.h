// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DA_Animations.h"
#include "GameFramework/Actor.h"

#include <optional>

#include "BrawlerBase.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UBoxComponent;
class UBrawlOverlay;
class AFinisher;

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
DECLARE_MULTICAST_DELEGATE(FOnAttributeChangedSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDirectionAttributeChangedSignature, ECombatDirection);

struct FAttackData
{
	float AttackPower;
	ECombatDirection AttackDirection;
	FGameplayTag HitReactionTag;
};

UCLASS()
class MYPROJECT_API ABrawlerBase : public AActor
{
	friend class UBrawlLogic;

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrawlerBase();

	inline bool IsKOd() const { return bKO; }
	inline FAttackData GetAttackData() const {return FAttackData(CurrentAttackPower,ActiveHand); }
	
	UFUNCTION(BlueprintCallable)
	FVector GetIKTargetLocation() const;

	//returns true if attack is possible
	inline bool CanAttack() const {return CurrentAttackPower >= ATTACK_THRESHOLD_VALUE;}

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Set location and attachent of IK target componen, so that it follows mesh animation
	UFUNCTION(BlueprintCallable)
	void SetIKTarget(FVector Location, FName Socket);
	
	UFUNCTION(BlueprintCallable)
	void PlayHitReaction();

	//	Increase current attack power by IncrementValue, if already restored to base value
	void IncrementAttackPower();

	//Set Attack Direction
	void SelectCombatDirection(ECombatDirection NewDirection);

	//Perform attack
	void Attack();
	
	//Same as attack, but acion is saved to form a code, that is compared with actual code
	void EnterFinisherCode();

	FAnimData FindAnimDataByTag(FGameplayTag AnimationTagToSearch);

	//Recieve an attack 
	void GetHit(FAttackData AttackData);

	//Block is held and ready to deflect incoming attack
	void HoldBlock();

	//Block button is released
	void ReleaseBlock();

	//Restore values to pre-combat state
	void Reset();

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
	void SetOverlay();

	void SpawnFinisher();

	//Apply skin material as dynamic material instance
	void CreateSkinDMI();

	//Returns absolute amount of incoming damage that is blocked by block
	float CalcDamageReduction(const FAttackData& AttackData) const;

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

	FOnAttributeChangedSignature OnFinisherUsed;
	FOnAttributeChangedSignature OnKOd;
	FOnAttributeChangedSignature OnDead;
	FOnAttributeChangedSignature OnReset;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> RightFistCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> LeftFistCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> HeadCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BodyCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> IKTargetComponent;

	//Direction that will be used to attack
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatDirection ActiveHand {ECombatDirection::None_Max};

	//Amount of damage that can be resisted now
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth {50.f};

	float MaxHealth{ CurrentHealth };

	//Amount of damage that can be resisted now
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CurrentBlockPower {0.f};
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//Amount of damage that can be dealt now
	float CurrentAttackPower {0.f};

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	//Amount of damage that can be dealt now
	bool bHoldingBlock {false};

	//Controls when hand IK is active during attack
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bUseHandIK {false};


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

	//List of possible finishers
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties, meta = (AllowPrivateAccess = "true"))
	TSet<TSubclassOf<AFinisher>> Finishers;

# pragma endregion

	//Direction of current blocking animation.
	ECombatDirection BlockingAnimDirection { ECombatDirection::None_Max };

	//Info about the attack that we just received
	FAttackData HitData;

	bool bKO {false};
	bool bDead {false};
	bool bPlayingHitReaction {false};
};
