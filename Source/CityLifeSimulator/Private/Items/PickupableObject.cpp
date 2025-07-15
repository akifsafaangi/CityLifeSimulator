// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupableObject.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
APickupableObject::APickupableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsPickedUp = false;
	HighlightMaterial = nullptr;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Component"));
}

void APickupableObject::Interact_Implementation(AActor* Interactor)
{
	Pickup(Interactor);
}

// Called when the game starts or when spawned
void APickupableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickupableObject::Pickup(AActor* Picker)
{
	UPrimitiveComponent* RootPrim = FindComponentByClass<UPrimitiveComponent>();

	if (!RootPrim) return;

	CurrentInteractor = Picker;
	bIsPickedUp = true;

	AFPS_Character* Player = Cast<AFPS_Character>(Picker);
	if (Player && Player->GetCamera())
	{
		FVector GrabLocation = Player->GetCamera()->GetComponentLocation() + Player->GetCamera()->GetForwardVector() * 400.f;
		FRotator GrabRotation = FRotator::ZeroRotator;
		PhysicsHandle->GrabComponentAtLocationWithRotation(RootPrim, NAME_None, RootPrim->GetComponentLocation(), RootPrim->GetComponentRotation());
	}
}

// Called every frame
void APickupableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentInteractor && PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		AFPS_Character* Player = Cast<AFPS_Character>(CurrentInteractor);
		if (Player && Player->GetCamera())
		{
			FVector TargetLoc = Player->GetCamera()->GetComponentLocation() + Player->GetCamera()->GetForwardVector() * 400.f;
			FRotator TargetRot = Player->GetCamera()->GetComponentRotation();
			PhysicsHandle->SetTargetLocationAndRotation(TargetLoc, TargetRot);
		}
	}
}

void APickupableObject::InteractHighlight_Implementation()
{
	
}

void APickupableObject::InteractEnd_Implementation()
{
	// Logic for ending interaction can be implemented here if needed
}