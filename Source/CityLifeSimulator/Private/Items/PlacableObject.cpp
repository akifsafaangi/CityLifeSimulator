// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PlacableObject.h"

// Sets default values
APlacableObject::APlacableObject()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    bIsInPlacementMode = false;
}

// Called when the game starts or when spawned
void APlacableObject::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void APlacableObject::Tick(float DeltaTime)
{

}


FItemDetails APlacableObject::getItem() const
{
    return itemInfo;
}

void APlacableObject::setItem(FItemDetails item)
{
    itemInfo = item;
}

void APlacableObject::Interact_Implementation(AActor* Interactor)
{
    if (!bIsInPlacementMode)
    {
        EnterPlacementMode(Interactor);
    }
    else
    {
        Place();
    }
}

void APlacableObject::Place()
{
    bIsInPlacementMode = false;
    bIsPickedUp = false;
    SetCurrentInteractor(nullptr);
}

void APlacableObject::EnterPlacementMode(AActor* Interactor)
{
    bIsInPlacementMode = true;
    Interactor->SetActorTickEnabled(true);
}

void APlacableObject::UpdatePlacement(const FVector& HitLocation, const FVector& HitNormal)
{
    FVector Origin, Extent;
    GetActorBounds(true, Origin, Extent);

    FVector PlaceLocation = HitLocation;
    PlaceLocation.Z += Extent.Z;

    SetActorLocation(PlaceLocation);
    SetActorRotation(FRotator(0, PlacementYawOffset, 0));
}

void APlacableObject::RotateDuringPlacement(float Value)
{
    PlacementYawOffset += Value * 2.0f;
}