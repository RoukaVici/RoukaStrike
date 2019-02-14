// Fill out your copyright notice in the Description page of Project Settings.

#include "RoukaViciManager.h"

#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "RoukaViciWidget.h"

URoukaViciManager *URoukaViciManager::instance = NULL;
URoukaViciWidget *URoukaViciManager::UI = NULL;

// Sets default values
URoukaViciManager::URoukaViciManager()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    
    instance = this;
	patterns = this->patterns;
}

// Called when the game starts or when spawned
void URoukaViciManager::BeginPlay()
{
	Super::BeginPlay();
	ULibRoukaVici::LoadLib();
}

// Called every frame
void URoukaViciManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URoukaViciManager::BeginDestroy()
{
	Super::BeginDestroy();
	//ULibRoukaVici::CallStopLib();
}

void URoukaViciManager::DisplayUI()
{
	UI->SetVisibility(ESlateVisibility::Visible);
}

void URoukaViciManager::HideUI()
{
	UI->SetVisibility(ESlateVisibility::Hidden);
}

void URoukaViciManager::ToggleUI()
{
	UI->toggleUIDelegate.Broadcast();
}

void URoukaViciManager::SetVibrationPattern(int ID)
{
	patternID = ID;
}

void URoukaViciManager::SavePattern(const FmPattern &pattern, int editedPattern)
{
	// Get the right folder path
	FString folderPath;
	if (GetWorld()->WorldType == EWorldType::PIE)
	{
		folderPath = FPaths::ProjectDir();
		FPaths::NormalizeDirectoryName(folderPath);
	}
	folderPath += "/Vibration Patterns/";
	FString newPattern = folderPath + pattern.name + ".json";
	FString OutputString;

	if (editedPattern < 0)
	{
		// New pattern to save
		patterns.Add(pattern);
		FJsonObjectConverter::UStructToJsonObjectString(pattern, OutputString, 0, 0, 0, NULL, true);
	}
	else
	{
		// Edited Pattern has to replace the previous one
		FString oldPattern = folderPath + patterns[editedPattern].name + ".json";
		if (oldPattern != newPattern)
			FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*oldPattern);

		FJsonObjectConverter::UStructToJsonObjectString(pattern, OutputString, 0, 0, 0, NULL, true);
		patterns[editedPattern] = pattern;
	}

	// Write in the Json file
	FFileHelper::SaveStringToFile(OutputString, *newPattern);

	// Tell the UI that it has to update its content
	updateUIDelegate.Broadcast();
}

void URoukaViciManager::DeletePattern(int patternToDelete)
{
	// Get the right folder path
	FString folderPath;
	if (GetWorld()->WorldType == EWorldType::PIE)
	{
		folderPath = FPaths::ProjectDir();
		FPaths::NormalizeDirectoryName(folderPath);
	}
	folderPath += "/Vibration Patterns/";

	// Make sure it's in range
	if (patternToDelete >= patterns.Num() || patternToDelete < 0)
		return;

	// Delete the Json file
	FString pattern = folderPath + patterns[patternToDelete].name + ".json";
	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*pattern);

	// Remove the pattern from the manager
	patterns.RemoveAt(patternToDelete);
}
