// Fill out your copyright notice in the Description page of Project Settings.

#include "VibrationPatternParser.h"
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

#include "RoukaViciWidget.h"
#include "RoukaViciManager.h"

FmPattern::FmPattern()
{
	name = "";
	duration = 0;
	FmMotor elem;
	motors.Init(elem, (int)EMotorNumber::MN_LastMotor);
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
        ParseData();
		URoukaViciManager::instance->patterns = patterns;
	}
	else
		patterns = URoukaViciManager::instance->patterns;

	// Instantiate the UI Widget.
	URoukaViciManager::UI = CreateWidget<URoukaViciWidget>(GetOwner()->GetGameInstance(), widgetTemplate);
	if (URoukaViciManager::UI)
	{
		URoukaViciManager::UI->AddToViewport();
		URoukaViciManager::UI->patterns = patterns;
		URoukaViciManager::UI->manager = URoukaViciManager::instance;
		URoukaViciManager::UI->createUIDelegate.Broadcast();
	}
}

void UVibrationPatternParser::CheckPatternsFolder(const FString &folderPath)
{
	if (!FPaths::FileExists(folderPath + "Default.json"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Vibration Patterns not found, creating default profile"));

		// Instantiate a default pattern
		FmPattern elemP;
		elemP.name = "Default";
		elemP.duration = 0.5f;
		for (int i = 0; i < 10; ++i)
		{
			FmMotor elemF;
			elemF.id = i;
			elemF.pattern.Add(80);
			elemF.pattern.Add(90);
			elemP.motors[i] = elemF;
		}

		// Write a Json file in the folder.
		FString OutputString;
		FJsonObjectConverter::UStructToJsonObjectString(elemP, OutputString, 0, 0, 0, NULL, true);
		FFileHelper::SaveStringToFile(OutputString, *(folderPath + "Default.json"));
	}
}

void UVibrationPatternParser::ParseData()
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
	CheckPatternsFolder(folderPath);

	// Gets all the Json files in the folder
	TArray<FString> Files;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.FindFiles(Files, *(folderPath + "*.json"), true, false);

	// Limit the amount of patterns to parse
	int patternNbLimit = 10;
	for (FString path: Files)
	{
		if (patternNbLimit-- < 0)
			break;
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
			float duration = parsedObject->GetNumberField(TEXT("duration"));
			TArray<TSharedPtr<FJsonValue> > motors = parsedObject->GetArrayField(TEXT("motors"));

			// Check if the name, duration and motors count are valid. Skip the file if not valid.
			if (name.Len() == 0 || duration <= 0 || motors.Num() != (int)EMotorNumber::MN_LastMotor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Your config file must have a valid name, a positive duration and 10 motors"));
				continue;
			}

			//  Add an element to the pattern array and fill it.
			int i = 0;
			elemP.name = name;
			elemP.duration = duration;
			for (TSharedPtr<FJsonValue> f : motors)
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
				elemP.motors[i++] = elemF;
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

