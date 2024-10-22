// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Finisher.generated.h"

/*
* This class contains execution of the combat finisher
*/

class ABrawl;

UCLASS()
class MYPROJECT_API AFinisher : public AActor
{
	GENERATED_BODY()
	
public:	
	AFinisher();
	void SetBrawl(ABrawl* NewBrawl) {Brawl = NewBrawl;};

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABrawl> Brawl;
};
