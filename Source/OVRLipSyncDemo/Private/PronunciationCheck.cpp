// Fill out your copyright notice in the Description page of Project Settings.


#include "PronunciationCheck.h"
#include <cctype>
#include <algorithm>
using namespace std;

TArray<FString> UPronunciationCheck::CompareTexts(const FString &Text1, const FString &Text2)
{
	int index = 0;
	int sentenceLength = Text1.Len();
	int pronouncedLength = Text2.Len();
	TArray<FString> orginalWords;
	Text1.ParseIntoArray(orginalWords, TEXT(" "), true);
	TArray<FString> pronouncedWords;
	Text2.ParseIntoArray(pronouncedWords, TEXT(" "), true);
	bool flag = false;
	int traversalLength = orginalWords.Num() <= pronouncedWords.Num() ? orginalWords.Num() : pronouncedWords.Num();
	if (orginalWords.Num() != pronouncedWords.Num())
		flag = true;
	for (int Index = 0; Index < traversalLength; ++Index)
	{
		if (orginalWords[Index] == pronouncedWords[Index])
			pronouncedWords.Remove(orginalWords[Index]);
		else
			flag = true;

	}
	if (flag)
	{
		orginalWords.Insert("Pronounced Incorrectly - Focus on Words: ", 0);
		return orginalWords;

	}

	orginalWords.Insert("Pronounced Correctly", 0);

	return orginalWords;



}
