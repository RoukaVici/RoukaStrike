// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LibRoukaVici.generated.h"

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
	static bool loadLib();

	/**
	 * @brief Call the InitRVici function from the API.
	 * 
	 * @return int see the RoukaVici API's documentation for more information.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int initLib();

	/**
	 * @brief Call the StopRVici function from the API.
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static void callStopLib();

	/**
	 * @brief Call the Vibrate function from the API.
	 * 
	 * @param motor the ID of the motor.
	 * @param ensity the intensity of the vibration between 0 and 255
	 * @return int see the RoukaVici API's documentation for more information.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int callVibrate(int motor, int intensity);

	/**
	 * @brief Call the SetLogMode function from the API.
	 * 
	 * @param mode see the RoukaVici API's ddocumentation for more information.
	 */
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static void callSetLogMode(const int mode);

	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int callChangeDeviceManager(const int dm);
	
	UFUNCTION(BlueprintCallable, Category = "Lib RoukaVici")
	static int callFindDevice();
};
