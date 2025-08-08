// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/PlacableObject.h"
#include "Components/BoxComponent.h"
#include "CardboardBox.h"
#include "StorageShelf.generated.h"

UCLASS()
class CITYLIFESIMULATOR_API AStorageShelf : public APlacableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorageShelf();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlaceObjects(UBoxComponent* sectionBox, ACardboardBox* Cardboard);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TArray<UBoxComponent*> SectionCollisions;
private:
	UBoxComponent* FindSection(UBoxComponent* BoxComponent) const;
};
