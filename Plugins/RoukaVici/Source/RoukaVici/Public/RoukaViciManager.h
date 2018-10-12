// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VibrationPatternParser.h"
#include "LibRoukaVici.h"
#include "RoukaViciManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateUIDelegate);

UENUM(BlueprintType)
enum class EMotorNumber : uint8
{
	MN_LeftPinky 	UMETA(DisplayName = "Left Pinky Motor"),
	MN_LeftRing 	UMETA(DisplayName = "Left Ring Motor"),
	MN_LeftMiddle	UMETA(DisplayName = "Left Middle Motor"),
	MN_LeftIndex	UMETA(DisplayName = "Left Index Motor"),
	MN_LeftThumb	UMETA(DisplayName = "Left Thumb Motor"),
	MN_RightThumb 	UMETA(DisplayName = "Right Thumb Motor"),
	MN_RightIndex 	UMETA(DisplayName = "Right Index Motor"),
	MN_RightMiddle	UMETA(DisplayName = "Right Middle Motor"),
	MN_RightRing	UMETA(DisplayName = "Right Ring Motor"),
	MN_RightPinky	UMETA(DisplayName = "Right Pinky Motor"),
	MN_LastMotor
};

/**
* \class ARoukaViciManager
*
* This class handles all the necessary functions
* and variables in order to run RoukaVici
* properly. A lot of objects in the world
* will need a reference to the instance of this class.
* You can set it up inside the properties window of the editor
*
* \author $Author: Louis Jahn $
*
* \version $Revision: 1.1 $
*
*/
/**
 * @brief This class stores and handles
 * all the necessary functions 
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROUKAVICI_API URoukaViciManager : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	URoukaViciManager();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void BeginDestroy() override;
    
	// Singleton
    static URoukaViciManager *instance;

	/**
	 * @brief Get the singleton's instance for RoukaVici Manager
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "RoukaVici Manager")
	static URoukaViciManager *GetInstance() { return instance; }

	// A pointer to the pattern editor Widget
	static UVibrationSelectionWidget *patternEditor;

	/**
	 * @brief Display the pattern editor.
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "RoukaVici Manager")
	static void DisplayPatternEditor();

	/**
	 * @brief Hide the pattern editor.
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "RoukaVici Manager")
	static void HidePatternEditor();

	/**
	* Select another pattern from the list.
	* This pattern will be used for the next
	* vibration command.
	*
	* @param patternID The ID of the pattern that is selected
	* @return void
	*/
	UFUNCTION(BlueprintCallable, Category = "RoukaVici Manager")
	void SetVibrationPattern(int patternID);

	// The array of registered patterns
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RoukaVici Manager")
	TArray<FmPattern> patterns;

	/**
	 * @brief Save the modified or newly created pattern
	 * to the current list of patterns.
	 * 
	 * @param pattern 
	 * @param eidtedPattern 
	 */
	UFUNCTION(BlueprintCallable, Category = "RoukaVici Manager")
	void SavePattern(const FmPattern &pattern, int eidtedPattern);

	// The ID of the currently selected pattern
	UPROPERTY(BlueprintReadOnly, Category = "RoukaVici Manager")
	int patternID = 0;

	// An event dispatcher that will update the content of the UI after a save or a pattern creation
	UPROPERTY(BlueprintAssignable, Category = "RoukaVici Manager")
	FUpdateUIDelegate updateUIDelegate;
};
