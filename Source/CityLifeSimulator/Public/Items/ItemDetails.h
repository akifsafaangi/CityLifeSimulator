// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDetails.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemDetails
{
	GENERATED_BODY()

	FItemDetails() : itemName(NAME_None), itemIcon(nullptr) {

	}

	FItemDetails(FName name, UTexture2D* icon) : itemName(name), itemIcon(icon) {

	}

	UPROPERTY(BlueprintReadWrite, Category = "Item Info")
	FName itemName;

	UPROPERTY(BlueprintReadWrite, Category = "Item Info")
	UTexture2D* itemIcon;

};
