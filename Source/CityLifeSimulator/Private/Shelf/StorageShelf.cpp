// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelf/StorageShelf.h"

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

void AStorageShelf::PlaceObjects()
{
			UE_LOG(LogTemp, Warning, TEXT("Placing objects in sectio"));
}