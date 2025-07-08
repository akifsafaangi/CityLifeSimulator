// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/ItemDetails.h"
#include "PlacableObject.generated.h"

UCLASS()
class CITYLIFESIMULATOR_API APlacableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlacableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FItemDetails itemInfo;

public:
	UFUNCTION(BlueprintCallable, Category = "ItemInfo")
	FItemDetails getItem() const;

	UFUNCTION(BlueprintCallable, Category = "ItemInfo")
	void setItem(FItemDetails item);

};
