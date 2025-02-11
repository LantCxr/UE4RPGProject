// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYFIRSTPROJECT_API UMyObject : public UObject
{
	GENERATED_BODY()

public:

	UMyObject();

	UPROPERTY(BlueprintReadWrite,Category = "MyVariables")
	float MyFloat;
	
	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void MyFunction();
};
