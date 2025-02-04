// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateFrameSequenceAsset.h"
#include "Misc/FileHelper.h"
#include "OVRLipSyncContextWrapper.h"
#include "LipSyncUtilsLibrary.h"
#include "OVRLipSyncFrame.h"
#include "Misc/FileHelper.h"


UCreateFrameSequenceAsset* UCreateFrameSequenceAsset::CookFrameSequenceAsset(
	FString WaveFilePath,
	bool bSaveToFile /*= true*/
) {
	UCreateFrameSequenceAsset* BPNode = NewObject<UCreateFrameSequenceAsset>();

	BPNode->WaveFilePath = WaveFilePath;
	BPNode->SaveToFile = bSaveToFile;

	return BPNode;
}

void UCreateFrameSequenceAsset::Activate()
{
    if (!FPaths::FileExists(WaveFilePath))
	{
		onFrameSequenceAssetCooked.Broadcast(nullptr, false);
		return;
	}

	TArray<uint8> RawSamples;
	if (!FFileHelper::LoadFileToArray(RawSamples, *WaveFilePath))
	{
		return;
	}

	FWaveModInfo waveInfo;
	uint8 *waveData = const_cast<uint8 *>(RawSamples.GetData());

	if (waveInfo.ReadWaveInfo(waveData, RawSamples.Num()))
	{

		auto LipSyncSequenceUpateFrequency = 100;
		auto LipSyncSequenceDuration = 1.0f / LipSyncSequenceUpateFrequency;

		int32 NumChannels = *waveInfo.pChannels;
		int32 SampleRate = *waveInfo.pSamplesPerSec;
		int32 bufferSize = 4096;
		auto PCMDataSize = waveInfo.SampleDataSize / sizeof(int16_t);
		int16_t *PCMData = reinterpret_cast<int16_t *>(waveData + 44);
		int32 ChunkSizeSamples = static_cast<int32>(SampleRate * LipSyncSequenceDuration);
		int32 ChunkSize = NumChannels * ChunkSizeSamples;

		FString modelPath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("OVRLipSync"), TEXT("OfflineModel"), TEXT("ovrlipsync_offline_model.pb"));

		Async(EAsyncExecution::Thread, [=]()
		{
			UOVRLipSyncFrameSequence *Sequence = NewObject<UOVRLipSyncFrameSequence>();
			UOVRLipSyncContextWrapper context(ovrLipSyncContextProvider_Enhanced, SampleRate, bufferSize,
											modelPath);
			
			float LaughterScore = 0.0f;
			int32_t FrameDelayInMs = 0;
			TArray<float> Visemes;

			TArray<int16_t> samples;
			samples.SetNumZeroed(ChunkSize);
			context.ProcessFrame(samples.GetData(), ChunkSizeSamples, Visemes, LaughterScore, FrameDelayInMs,
								 NumChannels > 1);

			int FrameOffset = (int)(FrameDelayInMs * SampleRate / 1000 * NumChannels);

			for (int offs = 0; offs < PCMDataSize + FrameOffset; offs += ChunkSize)
			{
				int remainingSamples = PCMDataSize - offs;
				if (remainingSamples >= ChunkSize)
				{
					context.ProcessFrame(PCMData + offs, ChunkSizeSamples, Visemes, LaughterScore, FrameDelayInMs,
										 NumChannels > 1);
				}
				else
				{
					if (remainingSamples > 0)
					{
						memcpy(samples.GetData(), PCMData + offs, sizeof(int16_t) * remainingSamples);
						memset(samples.GetData() + remainingSamples, 0, ChunkSize - remainingSamples);
					}
					else
					{
						memset(samples.GetData(), 0, ChunkSize);
					}
					context.ProcessFrame(samples.GetData(), ChunkSizeSamples, Visemes, LaughterScore, FrameDelayInMs,
										 NumChannels > 1);
				}

				if (offs >= FrameOffset)
				{
					Sequence->Add(Visemes, LaughterScore);
				}
			}

			if (SaveToFile)
			{
				// save asset to file next to `WaveFilePath`
				FString WaveBaseName = FPaths::GetBaseFilename(WaveFilePath);
				FString WaveFolder = FPaths::GetPath(WaveFilePath);
				FString Ext = FPaths::GetExtension(WaveFilePath);
				FString SequenceFileName = FPaths::Combine(WaveFolder, FString::Printf(TEXT("%s_sequence.fs"), *WaveBaseName));

				FString saveingError;
				if (!ULipSyncUtilsLibrary::SaveFrameSequenceToFile(Sequence, SequenceFileName, saveingError))
				{
					AsyncTask(ENamedThreads::GameThread, [this]() {
						onFrameSequenceAssetCooked.Broadcast(nullptr, false);
					});
					return;
				}
			}


			AsyncTask(ENamedThreads::GameThread, [Sequence, this]() {
				onFrameSequenceAssetCooked.Broadcast(Sequence, true);
			});
		});
	}
	else
		onFrameSequenceAssetCooked.Broadcast(nullptr, false);
}
