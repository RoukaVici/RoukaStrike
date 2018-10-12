// Fill out your copyright notice in the Description page of Project Settings.

#include "VibrationPatternParser.h"
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

#include "VibrationSelectionWidget.h"
#include "RoukaViciManager.h"

FmPattern::FmPattern()
{
	name = "";
	delay = 0;
	FmMotor elem;
	fingers.Init(elem, (int)EMotorNumber::MN_LastMotor);
}

// Sets default values
UVibrationPatternParser::UVibrationPatternParser()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called every frame
void UVibrationPatternParser::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Called when the game starts or when spawned
void UVibrationPatternParser::BeginPlay()
{
	Super::BeginPlay();
    
	// The parser doesn't do anything if the controller isn't instantiated in the level.
    if (URoukaViciManager::instance == NULL)
        return ;
    
	// Doesn't parse if the manager already has patterns, meaning that pasring has already been made.
	if (URoukaViciManager::instance->patterns.Num() == 0)
	{
        parseData();
		URoukaViciManager::instance->patterns = patterns;
	}
	else
		patterns = URoukaViciManager::instance->patterns;

	// Instantiate the Widget for the pattern editor, and display it.
	URoukaViciManager::patternEditor = CreateWidget<UVibrationSelectionWidget>(GetOwner()->GetGameInstance(), widgetTemplate);
	if (URoukaViciManager::patternEditor)
	{
		URoukaViciManager::patternEditor->AddToViewport();
		URoukaViciManager::patternEditor->patterns = patterns;
		URoukaViciManager::patternEditor->manager = URoukaViciManager::instance;
		URoukaViciManager::patternEditor->createUIDelegate.Broadcast();
	}
}

void UVibrationPatternParser::parseData()
{
	// Determine if we are currently in the editor or not.
	FString folderPath;
	if (GetWorld()->WorldType == EWorldType::PIE)
	{
		// The path for the patterns is the root of the project
		folderPath = FPaths::ProjectDir();
		FPaths::NormalizeDirectoryName(folderPath);
	}

	// If we are in the editor, complete the path. Otherwise this is the path.
	folderPath += "/Vibration Patterns/";

	// If the folder doesn't exist, create it with a default pattern
	if (!FPaths::FileExists(folderPath + "Default.json"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Vibration Patterns not found, creating default profile"));

		FmPattern elemP;
		elemP.name = "Default";
		elemP.delay = 0.5f;
		for (int i = 0; i < 10; ++i)
		{
			FmMotor elemF;
			elemF.id = i;
			elemF.pattern.Add(80);
			elemF.pattern.Add(90);
			elemP.fingers[i] = elemF;

		}

		// Write a Json file in the folder.
		FString OutputString;
		FJsonObjectConverter::UStructToJsonObjectString(elemP, OutputString, 0, 0, 0, NULL, true);
		FFileHelper::SaveStringToFile(OutputString, *(folderPath + "Default.json"));
	}

	// Gets all the Json files in the folder
	TArray<FString> Files;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.FindFiles(Files, *(folderPath + "*.json"), true, false);

	for (FString path: Files)
	{
		// Create the path for each Json file in the folder.
		FString data;
		FFileHelper::LoadFileToString(data, *(folderPath + path));
		TSharedPtr<FJsonObject> parsedObject;
		TSharedRef< TJsonReader<> > reader = TJsonReaderFactory<>::Create(data);

		// Deserialize into the pattern structure.
		FmPattern elemP;
		if (FJsonSerializer::Deserialize(reader, parsedObject))
		{
			FString name = parsedObject->GetStringField(TEXT("name"));
			float delay = parsedObject->GetNumberField(TEXT("delay"));
			TArray<TSharedPtr<FJsonValue> > fingers = parsedObject->GetArrayField(TEXT("fingers"));

			// Check if the name, delay and motors count are valid. Skip the file if not valid.
			if (name.Len() == 0 || delay <= 0 || fingers.Num() != (int)EMotorNumber::MN_LastMotor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Your config file must have a valid name, a positive delay and 10 fingers"));
				continue;
			}

			//  Add an element to the pattern array and fill it.
			int i = 0;
			elemP.name = name;
			elemP.delay = delay;
			for (TSharedPtr<FJsonValue> f : fingers)
			{
				FmMotor elemF;
				const TSharedPtr<FJsonObject> *fo;

				// Check if the Json structure matches the one in the file.
				if (!f->TryGetObject(fo))
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid motor format"));
					continue;
				}
				
				// Check for the order of the motor is in increasing order.
				int id = (*fo)->GetIntegerField(TEXT("id"));
				if (id != i)
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid motor ID, make sure it's in increasing order"));
					continue;
				}

				// Get the list of motors and fill its content into the list of patterns.
				elemF.id = id;
				TArray<TSharedPtr<FJsonValue> > fon = (*fo)->GetArrayField(TEXT("pattern"));
				if (fon.Num() == 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Your config file must have a positive pattern size"));
					continue ;
				}
				for (TSharedPtr<FJsonValue> fonb : fon)
				{
					int out;
					if (fonb->TryGetNumber(out))
						elemF.pattern.Add(out);
				}
				elemP.fingers[i++] = elemF;
			}
		}
		// If the deserialization doesn't succeed, skip the file
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid file / file extension"));
			continue ;
		}
		patterns.Add(elemP);
	}
}

