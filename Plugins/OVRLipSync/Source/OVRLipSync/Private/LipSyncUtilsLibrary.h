// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Sound/SoundWave.h"
#include "OVRLipSyncFrame.h"
#include "LipSyncUtilsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ULipSyncUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "LipSyncUtils")
		static bool SaveSoundToWaveFile(USoundWave* Sound, FString OutFilePath, FString& Error);

	UFUNCTION(BlueprintCallable, Category = "LipSyncUtils")
		static USoundWave* LoadWaveFromFile(FString WaveFilePath);

	UFUNCTION(BlueprintCallable, Category = "LipSyncUtils")
		static bool SaveFrameSequenceToFile(UOVRLipSyncFrameSequence* FrameSequence, FString FilePath, FString& Error);

	UFUNCTION(BlueprintCallable, Category = "LipSyncUtils")
		static UOVRLipSyncFrameSequence* LoadFrameSequenceFromFile(FString FilePath);

};
