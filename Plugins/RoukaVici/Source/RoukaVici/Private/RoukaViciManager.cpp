// Fill out your copyright notice in the Description page of Project Settings.

#include "RoukaViciManager.h"

#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "VibrationSelectionWidget.h"

URoukaViciManager *URoukaViciManager::instance = NULL;
UVibrationSelectionWidget *URoukaViciManager::patternEditor = NULL;

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
	ULibRoukaVici::loadLib();
}

// Called every frame
void URoukaViciManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URoukaViciManager::BeginDestroy()
{
	Super::BeginDestroy();
	ULibRoukaVici::callStopLib();
}

void URoukaViciManager::DisplayPatternEditor()
{
	patternEditor->SetVisibility(ESlateVisibility::Visible);
}

void URoukaViciManager::HidePatternEditor()
{
	patternEditor->SetVisibility(ESlateVisibility::Hidden);
}

void URoukaViciManager::SetVibrationPattern(int ID)
{
	patternID = ID;
}

void URoukaViciManager::SavePattern(const FmPattern &pattern, int editedPattern)
{
	FString folderPath;
	if (GetWorld()->WorldType == EWorldType::PIE)
	{
		folderPath = FPaths::ProjectDir();
		FPaths::NormalizeDirectoryName(folderPath);
	}
	folderPath += "/Vibration Patterns/";
	FString oldPattern = folderPath + patterns[editedPattern].name + ".json";
	FString newPattern = folderPath + pattern.name + ".json";

	if (oldPattern != newPattern)
		FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*oldPattern);

	FString OutputString;
	FJsonObjectConverter::UStructToJsonObjectString(pattern, OutputString, 0, 0, 0, NULL, true);
	FFileHelper::SaveStringToFile(OutputString, *newPattern);

	patterns[editedPattern] = pattern;

	updateUIDelegate.Broadcast();
}
