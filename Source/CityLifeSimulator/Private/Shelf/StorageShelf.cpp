// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelf/StorageShelf.h"
#include "Shelf/ShelfSlotItemComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AStorageShelf::AStorageShelf()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AStorageShelf::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStorageShelf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStorageShelf::PlaceObjects(UBoxComponent* sectionBox, ACardboardBox* Cardboard)
{
	UBoxComponent* Section = FindSection(sectionBox);
	if (Section)
	{
		UE_LOG(LogTemp, Warning, TEXT("Placing objects in section: %s"), *Section->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Placing cardboard box: %s"), *Cardboard->GetName());

		TArray<USceneComponent*> ChildComponents;
		Section->GetChildrenComponents(true, ChildComponents);
		for (USceneComponent* Child : ChildComponents)
		{
			UShelfSlotItemComponent* ShelfSlotItem = Cast<UShelfSlotItemComponent>(Child);
			// Check if the slot is occupied
			if (!ShelfSlotItem)
			{
				continue; // Skip if the slot is not a valid ShelfSlotItemComponent
			}

			// If the slot is not occupied, attach the cardboard box to it
			if (!ShelfSlotItem->bIsOccupied)
			{
				ShelfSlotItem->bIsOccupied = true;
				Cardboard->MoveObject(ShelfSlotItem->GetComponentLocation(), 1.0f);
				UE_LOG(LogTemp, Warning, TEXT("Cardboard box placed in slot: %s"), *ShelfSlotItem->GetName());


				return;
			}
		}
	}
}

UBoxComponent* AStorageShelf::FindSection(UBoxComponent* BoxComponent) const
{
	for (UBoxComponent* Section : SectionCollisions)
	{
		if (Section == BoxComponent)
		{
			return Section;
		}
	}
	return nullptr;
}