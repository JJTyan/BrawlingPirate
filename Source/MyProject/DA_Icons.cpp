// Fill out your copyright notice in the Description page of Project Settings.


#include "DA_Icons.h"

FIconData UDA_Icons::GetRandomDataByTag(FGameplayTag Tag)
{
	TArray<FIconData> FoundData;

	for (const FIconData& Data : Icons)
	{
		if (Data.Tag == Tag)
		{
			FoundData.Add(Data);
		}
	}
	//Icons must have at least 1 entry with provided tag
	check(FoundData.Num() != 0);
	return FoundData[FMath::RandRange(0, FoundData.Num()-1)];
}
