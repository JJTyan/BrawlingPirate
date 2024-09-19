// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementScheme.generated.h"

class USplineComponent;

UCLASS()
class MYPROJECT_API AMovementScheme : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovementScheme();

	//returns direction along the spline for the provided Location
	FVector GetMovementDirection(const FVector& ActorLocation) const;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USplineComponent> Rout;

};
