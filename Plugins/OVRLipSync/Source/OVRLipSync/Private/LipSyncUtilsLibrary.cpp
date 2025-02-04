// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncUtilsLibrary.h"
#include "Misc/FileHelper.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"


bool ULipSyncUtilsLibrary::SaveSoundToWaveFile(USoundWave* Sound, FString OutFilePath, FString& Error)
{
	FText err;
	if (!FFileHelper::IsFilenameValidForSaving(OutFilePath, err))
		Error = err.ToString();
		return false;


	return true;
	
}

USoundWave *ULipSyncUtilsLibrary::LoadWaveFromFile(FString WaveFilePath)
{
	// Assuming that wave file has header information

	if (!FPaths::FileExists(WaveFilePath))
		return nullptr;

	// load file to bytes
	TArray<uint8> rawData;
	if (!FFileHelper::LoadFileToArray(rawData, *WaveFilePath))
		return nullptr;

	USoundWave *sWave = nullptr;

	// reading wave information
	FWaveModInfo waveInfo;

#if ENGINE_MINOR_VERSION > 19
	uint8 *waveData = const_cast<uint8 *>(rawData.GetData());
#else
	const uint8 *waveData = rawData.GetData();
#endif

	if (waveInfo.ReadWaveInfo(waveData, rawData.Num()))
	{
		// Construct USoundWave and feed received bytes
		sWave = NewObject<USoundWave>();

		sWave->RawData.Lock(LOCK_READ_WRITE);
		void *lockedData = sWave->RawData.Realloc(rawData.Num());
		FMemory::Memcpy(lockedData, rawData.GetData(), rawData.Num());
		sWave->RawData.Unlock();

		// apply wave info
		int32 DurationDiv = *waveInfo.pChannels * *waveInfo.pBitsPerSample * *waveInfo.pSamplesPerSec;
		if (DurationDiv)
			sWave->Duration = *waveInfo.pWaveDataSize * 8.0f / DurationDiv;
		else
			sWave->Duration = 0.0f;
		sWave->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
		sWave->NumChannels = *waveInfo.pChannels;
		sWave->RawPCMDataSize = waveInfo.SampleDataSize;

#if ENGINE_MINOR_VERSION < 19
		//sWave->SampleRate = *waveInfo.pSamplesPerSec;
#else
		sWave->SetSampleRate(*waveInfo.pSamplesPerSec);
#endif

		sWave->bLooping = false;
		sWave->RawPCMData = (uint8 *)FMemory::Malloc(sWave->RawPCMDataSize);
		const uint8 headerOffset = 44;
		FMemory::Memcpy(sWave->RawPCMData, rawData.GetData() + headerOffset, rawData.Num() - headerOffset);
	}

	return sWave;
}


bool ULipSyncUtilsLibrary::SaveFrameSequenceToFile(UOVRLipSyncFrameSequence *FrameSequence, FString FilePath, FString& Error)
{
	FText err;
	if (!FFileHelper::IsFilenameValidForSaving(FilePath, err))
	{
		Error = err.ToString();
		return false;
	}

	if (FrameSequence->Num() == 0)
	{
		Error = TEXT("Nothing to save");
		return false;
	}

	FBufferArchive buffer;
	for (FOVRLipSyncFrame& fs : FrameSequence->FrameSequence)
	{
		buffer << fs.LaughterScore;
		buffer << fs.VisemeScores;
	}

	if (FFileHelper::SaveArrayToFile(buffer, *FilePath))
	{
		buffer.FlushCache();
		buffer.Empty();
		return true;
	}

	return false;
}

UOVRLipSyncFrameSequence *ULipSyncUtilsLibrary::LoadFrameSequenceFromFile(FString FilePath)
{
	if (!FPaths::FileExists(FilePath))
		return nullptr;

	TArray<uint8> rawData;
	UOVRLipSyncFrameSequence* out = NewObject<UOVRLipSyncFrameSequence>();
	if (FFileHelper::LoadFileToArray(rawData, *FilePath))
	{
		FMemoryReader reader = FMemoryReader(rawData);

		while (!reader.AtEnd())
		{
			float LaughterScore;
			TArray<float> VisemeScores;

			reader << LaughterScore << VisemeScores;

			out->Add(VisemeScores, LaughterScore);
		}

		if (!reader.Close())
		{
			return nullptr;
		}
	}

	return out;
}