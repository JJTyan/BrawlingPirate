// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementScheme.h"
#include "Components/SplineComponent.h"

AMovementScheme::AMovementScheme()
{
	PrimaryActorTick.bCanEverTick = false;
	Rout = CreateDefaultSubobject<USplineComponent>(TEXT("Rout"));
	SetRootComponent(Rout);

}

FVector AMovementScheme::GetMovementDirection(const FVector& ActorLocation) const
{
	const FVector locOnSpline{ Rout->FindLocationClosestToWorldLocation(ActorLocation,ESplineCoordinateSpace::World) };
	return Rout->FindDirectionClosestToWorldLocation(locOnSpline,ESplineCoordinateSpace::World);
}

void AMovementScheme::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMovementScheme::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

