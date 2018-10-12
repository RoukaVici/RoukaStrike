// Fill out your copyright notice in the Description page of Project Settings.

#include "RoukaViciMotor.h"
#include "LibRoukaVici.h"


ARoukaViciMotor::ARoukaViciMotor()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARoukaViciMotor::BeginPlay()
{
	Super::BeginPlay();
    manager = URoukaViciManager::instance;	
}

// Called every frame
void ARoukaViciMotor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mTime += DeltaTime;
	
	// Verify that the delay elapsed
	if (isVibrating && id >= 0 && manager != NULL && manager->patterns.Num() > 0
		&& mTime >= manager->patterns[manager->patternID].delay)
	{
		// Find the intensity of the next vibration
		int currentStepIntensity = manager->patterns[manager->patternID].fingers[id].pattern[currentStepIndex];

		// Some debugging information
		UE_LOG(LogTemp, Warning, TEXT("Vibrating motor ID %d for an intensity of %d"), id, (255 * currentStepIntensity) / 100);
        
		// Vibrate at a specific calculated intensity
		ULibRoukaVici::callVibrate(id, (255 * currentStepIntensity) / 100);

		// Iterate in the steps provided in the patterns
		currentStepIndex = (currentStepIndex == manager->patterns[manager->patternID].fingers[id].pattern.Num() - 1 ? 0 : ++currentStepIndex);

		// Reset the timer and wait for a new iteration to begin
		mTime = 0;
	}
}

void ARoukaViciMotor::sendVibrationCommand(int fingerId)
{
	UE_LOG(LogTemp, Warning, TEXT("Vibrating for motor with id: %d"), fingerId);
	isVibrating = true;
	id = fingerId;
}

void ARoukaViciMotor::stopVibrationCommand(int fingerId)
{
	ULibRoukaVici::callVibrate(id, 0);
	UE_LOG(LogTemp, Warning, TEXT("Stopped Vibrating for motor with id: %d"), fingerId);
	isVibrating = false;
	id = fingerId;
	currentStepIndex = 0;
}

