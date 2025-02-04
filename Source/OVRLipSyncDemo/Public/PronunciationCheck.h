// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PronunciationCheck.generated.h"

/**
 * 
 */
UCLASS()
class OVRLIPSYNCDEMO_API UPronunciationCheck : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "TextComparison")
	static TArray<FString> CompareTexts(const FString &Text1, const FString &Text2);
};
