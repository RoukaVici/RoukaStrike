// Fill out your copyright notice in the Description page of Project Settings.

#include "LibRoukaVici.h"
#include "RoukaVici/include/RoukaViciAPI.h"


void CallbackDebug(const char *str)
{
	FString msg(str);
	UE_LOG(LogTemp, Warning, TEXT("RVLog: %s"), *msg);
}

bool ULibRoukaVici::loadLib()
{
	SetLogMode(2);
	RegisterDebugCallback(&CallbackDebug);

	if (InitRVici())
	{
		UE_LOG(LogTemp, Warning, TEXT("Error while loading Lib RoukaVici"));
		return false;
	}
	ChangeDeviceManager(2);
	FindDevice();

	UE_LOG(LogTemp, Warning, TEXT("Lib RoukaVici was successfuly loaded"));
	return true;
}

int ULibRoukaVici::initLib()
{
	return (InitRVici());
}

void ULibRoukaVici::callStopLib()
{
	StopRVici();
}

int ULibRoukaVici::callVibrate(int motor, int intensity)
{
	return Vibrate(motor, intensity);
}

void ULibRoukaVici::callSetLogMode(const int mode)
{
	SetLogMode(mode);
}

int ULibRoukaVici::callChangeDeviceManager(const int dm)
{
	return (ChangeDeviceManager(dm));
}

int ULibRoukaVici::callFindDevice()
{
	return (FindDevice());
}
