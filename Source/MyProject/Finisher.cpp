// Fill out your copyright notice in the Description page of Project Settings.


#include "Finisher.h"

AFinisher::AFinisher()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFinisher::BeginPlay()
{
	Super::BeginPlay();
}

void AFinisher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

