// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerAnimInstance.h"

void UBrawlerAnimInstance::InitializeAnimation(ABrawlerBase* Brawler)
{
	Owner = Brawler;

	Brawler->OnDirectionChanged.AddUFunction(this, FName("GetStanceDirection"));
}

void UBrawlerAnimInstance::GetStanceDirection(ECombatDirection NewDirection)
{
	LeftRightTarget = 0;
	TopBottomTarget = 0;

	switch (NewDirection)
	{
	case ECombatDirection::Top:
		TopBottomTarget = 100.f;
		break;
	case ECombatDirection::Bottom:
		TopBottomTarget = -100.f;
		break;
	case ECombatDirection::Left:
		LeftRightTarget = -100.f;
		break;
	case ECombatDirection::Right:
		LeftRightTarget = 100.f;
		break;
	default:
		break;
	}
}
