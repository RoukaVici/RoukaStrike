// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Multithreaded.h"
#include "Async/AsyncWork.h"
#include "LibRoukaVici.generated.h"

/**
 * @brief Basic thread pool implementation
 *
 */
class MultiThreadedTask : public FNonAbandonableTask
{
public:
	MultiThreadedTask(UObject *object) : object(object) {}

	UObject *object;

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(MultiThreadedTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork()
	{
		IMultithreaded::Execute_MultiThreadedFunction(object);
	}
};

/**
 * @brief The Blueprint function library containing
 * symbols to the RoukaVici API.
 * Functions can be either called from C++ or from Blueprints.
 * 
 */
UCLASS()
class ROUKAVICI_API ULibRoukaVici : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief Loads the symbols from the RoukaVici API.
	 * 
	 * @return true if successful.
	 * @return false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static bool LoadLib();

	/**
	 * @brief Call the InitRVici function from the API.
	 * 
	 * @return int see the RoukaVici API's documentation for more information.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int InitLib();

	/**
	 * @brief Call the StopRVici function from the API.
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static void CallStopLib();

	/**
	 * @brief Call the Vibrate function from the API.
	 * 
	 * @param motor the ID of the motor.
	 * @param ensity the intensity of the vibration between 0 and 255
	 * @return int see the RoukaVici API's documentation for more information.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int CallVibrate(int motor, int intensity);

	/**
	 * @brief Call the SetLogMode function from the API.
	 * 
	 * @param mode see the RoukaVici API's ddocumentation for more information.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static void CallSetLogMode(const int mode);

	/**
	 * @brief Call the ChangeDeviceManager function from the API.
	 *
	 * @param mode see the RoukaVici API's ddocumentation for more information.
	 * @return int see the RoukaVici API's documentation for more information.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int CallChangeDeviceManager(const int dm);
	
	/**
	 * @brief Call the FindeDevice function from the API.
	 *
	 * @return int see the RoukaVici API's documentation for more information.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int CallFindDevice();

	/**
	 * @brief Call the Status function from the API.
	 *
	 * @return int see the RoukaVici API's documentation for more information.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int CallStatus();

	/**
	 * @brief Change device manager to bluetooth and search for devices.
	 *
	 * @return 0 on success, otherwise refer to RoukaVici API's documentation.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int TryConnectingWithBluetooth();

	/**
	 * @brief Makes the calling object able to give work to a thread.
	 *
	 * @param A pointer to the UObject that will give work to do for the tread.
	 */
	UFUNCTION(BlueprintCallable)
	static void CallMultithreadedFunction(UObject *object);
};
