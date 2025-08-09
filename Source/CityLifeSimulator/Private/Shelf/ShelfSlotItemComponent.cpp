// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelf/ShelfSlotItemComponent.h"

// Sets default values for this component's properties
UShelfSlotItemComponent::UShelfSlotItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsOccupied = false;
	//ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	// ...
}


// Called when the game starts
void UShelfSlotItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShelfSlotItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLerping)
    {
        LerpElapsedTime += DeltaTime;
        float Alpha = FMath::Clamp(LerpElapsedTime / LerpDuration, 0.0f, 1.0f);

        FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
        ItemMesh->SetWorldLocation(NewLocation);

        if (Alpha >= 1.0f)
        {
			bIsOccupied = false;
            bIsLerping = false;
        }

		//StartLocation = GetActorLocation(); // Goes faster?
    }
}

void UShelfSlotItemComponent::MoveObject(FVector NewTargetLocation, float Duration)
{
	if (Duration <= 0.0f) return;

	GetChildrenComponents<UStaticMeshComponent>(true, ItemMesh);
	if (ItemMesh)
	{
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		StartLocation = ItemMesh->GetComponentLocation();
		TargetLocation = NewTargetLocation;
		LerpElapsedTime = 0.0f;
		LerpDuration = Duration;
		bIsLerping = true;
	}

}