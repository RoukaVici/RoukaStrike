// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RoukaViciManager.h"

#include "RoukaViciMotor.generated.h"

/**
* \class ARoukaViciMotor
*
* \brief Provide an example
*
* This class is the parent class of blueprint
* Finger. It is used to pass the
* variables so that the internal compenents in
* the inherited class can work properly with RoukaVici
*
* \author $Author: Louis Jahn $
*
* \version $Revision: 1.1 $
*
*/
UCLASS()
class ROUKAVICI_API ARoukaViciMotor : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	ARoukaViciMotor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RoukaVici Motor")
	URoukaViciManager *manager = NULL;

	/**
	* Start the vibration command for the associated motor
	*
	* @param fingerID The ID of the finger targeted
	* @return void
	*/
	UFUNCTION(BlueprintCallable, Category = "RoukaVici Motor")
	void sendVibrationCommand(int fingerID);

	/**
	* Stop the vibration command for the associated motor
	*
	* @param fingerID The ID of the targeted motor
	* @return void
	*/
	UFUNCTION(BlueprintCallable, Category = "RoukaVici Motor")
	void stopVibrationCommand(int fingerId);

private:

	// A boolean that indicates if the motor is currently vibrating
	bool isVibrating = false;

 	// The ID of the motor
	int id = -1;

	// An aray of int representing the iterations of the pattern
    //TArray<int> steps;

	 // An Integer that indicates the current step index
	int currentStepIndex = 0;

	// A Float used to define whether the delay from the pattern has elapsed. 
	float mTime = 0;
};
