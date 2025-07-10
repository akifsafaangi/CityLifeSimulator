// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ContainerBox.h"

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
	bIsPickedUp = false;
	
}


void AContainerBox::OpenBox()
{
}