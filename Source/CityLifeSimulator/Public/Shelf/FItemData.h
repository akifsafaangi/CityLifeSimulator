// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* Mesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UMaterialInterface* Material = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 1;
};
