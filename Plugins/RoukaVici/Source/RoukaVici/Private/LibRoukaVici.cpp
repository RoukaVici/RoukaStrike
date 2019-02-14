// Fill out your copyright notice in the Description page of Project Settings.

#include "LibRoukaVici.h"
#include "RoukaVici/include/RoukaViciAPI.h"


void CallbackDebug(const char *str)
{
	FString msg(str);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
}

bool ULibRoukaVici::LoadLib()
{
	SetLogMode(2);
	RegisterDebugCallback(&CallbackDebug);

	if (InitRVici())
	{
		UE_LOG(LogTemp, Warning, TEXT("Error while loading Lib RoukaVici"));
		return (false);
	}

	UE_LOG(LogTemp, Warning, TEXT("Lib RoukaVici was successfuly loaded"));
	return (true);
}

int ULibRoukaVici::InitLib()
{
	return (InitRVici());
}

void ULibRoukaVici::CallStopLib()
{
	StopRVici();
}

int ULibRoukaVici::CallVibrate(int motor, int intensity)
{
	return (Vibrate(motor, intensity));
}

void ULibRoukaVici::CallSetLogMode(const int mode)
{
	SetLogMode(mode);
}

int ULibRoukaVici::CallChangeDeviceManager(const int dm)
{
	return (ChangeDeviceManager(dm));
}

int ULibRoukaVici::CallFindDevice()
{
	return (FindDevice());
}

int ULibRoukaVici::CallStatus()
{
	return (Status());
}

int ULibRoukaVici::TryConnectingWithBluetooth()
{
	ChangeDeviceManager(2);
	return (FindDevice());
}

void ULibRoukaVici::CallMultithreadedFunction(UObject *object)
{
	(new FAutoDeleteAsyncTask<MultiThreadedTask>(object))->StartBackgroundTask();
}