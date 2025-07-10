// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ContainerBox.h"
#include "Items/PlacableObject.h"

// Sets default values
AContainerBox::AContainerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AContainerBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AContainerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AContainerBox::Interact_Implementation(AActor* Interactor)
{

	if (bIsPickedUp) {
		Throw();
		bIsPickedUp = false;
	}
	else {
		Pickup(Interactor);
		bIsPickedUp = true;
	}
}

void AContainerBox::Throw()
{
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
	}
	SetCurrentInteractor(nullptr);
}


void AContainerBox::OpenBox(AActor* Interactor)
{
	if (!ContainedObjectClass) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();

    APlacableObject* SpawnedObj = GetWorld()->SpawnActor<APlacableObject>(ContainedObjectClass, SpawnLocation, SpawnRotation, SpawnParams);

	AFPS_Character* FPSChar = Cast<AFPS_Character>(Interactor);
	if (SpawnedObj && FPSChar)
    {
		UE_LOG(LogTemp, Warning, TEXT("Spawned Placable Object: %s"), *SpawnedObj->GetName());
		// Clear the held actor reference in the character
		FPSChar->HeldActor = nullptr; // Clear any held actor
        SpawnedObj->EnterPlacementMode(FPSChar);
		FPSChar->bIsHolding = true;
        FPSChar->bIsInPlacementMode = true;
        FPSChar->PlacingActor = SpawnedObj;
	}

    Destroy();
}