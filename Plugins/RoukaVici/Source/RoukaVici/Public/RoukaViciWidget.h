// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "VibrationPatternParser.h"

#include "RoukaViciWidget.generated.h"


/**
* \class UVibrationSelectionWidget
*
* \brief Provide an example
*
* This class is the parent class of blueprint
* VibrationSelectionWidget. It is used to pass the
* variables so that the internal compenents in
* the inherited class can work properly with RoukaVici
*
* \author $Author: Louis Jahn $
*
* \version $Revision: 1.1 $
*
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCreateUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FToggleUIDelegate);

UCLASS()
class ROUKAVICI_API URoukaViciWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// The arrat of registered patterns
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoukaVici UI")
	TArray<FmPattern> patterns;

	// A pointer to the manager. Used for neater and cleaner Blueprints / C++ code.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoukaVici UI")
	URoukaViciManager *manager;

	// An event dispatcher used to initalize content in the UI (After pattern parsing)
	UPROPERTY(BlueprintAssignable, Category = "RoukaVici UI")
	FCreateUIDelegate createUIDelegate;

	// An event dispatcher used to toggle the UI
	UPROPERTY(BlueprintAssignable, Category = "RoukaVici UI")
	FToggleUIDelegate toggleUIDelegate;

protected:

	virtual void NativeConstruct() override;
	
};
