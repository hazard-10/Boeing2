// Fill out your copyright notice in the Description page of Project Settings.


#include "FileIOManager.h"

bool UFileIOManager::SaveArrayText(FString SaveDirectory, FString FileName, TArray<FString> SaveText, bool AllowOverWriting = false)
{
	// Set file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory)) {
			return false;
		}
	}

	FString FinalString = "";
	for (FString& Each : SaveText) {
		FinalString += Each;
		FinalString += LINE_TERMINATOR;
	}
	//EFileWrite::Type WriteMode = EFileWrite::APPEND;

	//FString loadContent = "";
	//TArray<FString> LoadExistingContent;
	//FFileHelper::LoadFileToString(loadContent, *SaveDirectory);


	//for (FString& Each : LoadExistingContent) {
	//	FinalString += Each;
	//	FinalString += LINE_TERMINATOR;
	//}
	//FinalString += loadContent;

	return FFileHelper::SaveStringToFile(FinalString, *SaveDirectory, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), 0x08);
}