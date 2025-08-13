// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelf/StorageShelf.h"
#include "Shelf/ShelfSlotItemComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AStorageShelf::AStorageShelf()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bIsInteracting = false;
}

// Called when the game starts or when spawned
void AStorageShelf::BeginPlay()
{
	Super::BeginPlay();
	
	for (UBoxComponent* coll : SectionCollisions) {
		TArray<USceneComponent*> children;
		coll->GetChildrenComponents(false, children);
		for (USceneComponent* child : children)
		{
			UShelfSlotItemComponent* shelfChild = Cast<UShelfSlotItemComponent>(child);
			if (shelfChild)
			{
				shelfChild->ClearItemData();
			}
		}
	}
}

// Called every frame
void AStorageShelf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStorageShelf::PlaceObjects(UBoxComponent* sectionBox, ACardboardBox* Cardboard, bool bDirection)
{
	UBoxComponent* Section = FindSection(sectionBox);
	if (Section)
	{
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
			{
				FScopeLock Lock(&CriticalSection);
				// If the slot is not occupied, attach the cardboard box to it
				if (!ShelfSlotItem->bIsMoving)
				{
					UE_LOG(LogTemp, Warning, TEXT("bDirection: %d, bGetStaticMesh: %s"), bDirection, ShelfSlotItem->VisualMesh->GetStaticMesh() ? TEXT("true") : TEXT("false"));
					if ((!ShelfSlotItem->VisualMesh->GetStaticMesh() && bDirection) || (ShelfSlotItem->VisualMesh->GetStaticMesh() && !bDirection)) {
						Cardboard->MoveObject(ShelfSlotItem, 1.0f, bDirection);
						return;
					}
				}
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