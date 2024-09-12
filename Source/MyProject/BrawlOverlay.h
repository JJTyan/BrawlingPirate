// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrawlOverlay.generated.h"

/**
 * 
 */

 class UBrawlWidgetController;

UCLASS()
class MYPROJECT_API UBrawlOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWidgetController(UBrawlWidgetController* NewWidgetController);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BindToController();

private:
	UPROPERTY(BlueprintReadOnly, Category = Setup, meta = (AllowPrivateAccess = "true"))
	UBrawlWidgetController* WidgetController;
	
};
