// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CreateFrameSequenceAsset.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFrameSequenceAssetCoocked, UOVRLipSyncFrameSequence*, FrameSequence, bool, Success);

/**
 * 
 */
UCLASS()
class OVRLIPSYNC_API UCreateFrameSequenceAsset : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "LipSync")
		FFrameSequenceAssetCoocked onFrameSequenceAssetCooked;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "LipSync")
		static UCreateFrameSequenceAsset* CookFrameSequenceAsset(FString WaveFilePath, bool bSaveToFile = true);

	FString WaveFilePath;
	bool SaveToFile = true;

	virtual void Activate() override;
};
