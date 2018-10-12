// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FileHelper.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "VibrationPatternParser.generated.h"

class UVibrationSelectionWidget;
class URoukaViciManager;

/**
* This struct store pattern information
* for the behavior a specific motor
*/
USTRUCT(BlueprintType)
struct FmMotor
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int id; ///< ID of the motor

	UPROPERTY(BluePrintReadWrite, VisibleAnywhere)
	TArray<int> pattern; ///< Array of value representing different intensities
};

USTRUCT(BlueprintType)
/**
 * @brief This struct stores motors patterns
 * with a name and a delay
 * 
 */
struct FmPattern
{
	GENERATED_USTRUCT_BODY()

	FmPattern();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString name; ///< Name of the pattern

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float delay; ///< delay between each intensities

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<FmMotor> fingers; ///< List of fingers for the pattern
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
/**
 * @brief This class parses the JSON files to 
 * get each patterns informations for each motor.
 * The parsed elements will be transmitted to the manager.
 * It will also instantiate the widget to personalize
 * these patterns.
 * 
 */
class ROUKAVICI_API UVibrationPatternParser : public UActorComponent
{
	GENERATED_BODY()
	
public:	
    
    UVibrationPatternParser();

	// Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// The type of Widget to instantiated for the UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RoukaVici UI")
	TSubclassOf<UUserWidget> widgetTemplate;

	// Array of parsed patterns
	UPROPERTY(VisibleAnywhere, Category = "RoukaVici UI")
	TArray<FmPattern> patterns;

protected:
	/// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/**
	 * @brief Parse the config files to get patterns.
	 * The folder is located at the root of the project.
	 * Called internaly at the instantiation of the object
	 * 
	 */
	void parseData();
};
