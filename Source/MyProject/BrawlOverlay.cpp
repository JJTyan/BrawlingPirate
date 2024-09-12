// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlOverlay.h"

void UBrawlOverlay::SetWidgetController(UBrawlWidgetController* NewWidgetController)
{
	WidgetController = NewWidgetController;
	BindToController();
}
