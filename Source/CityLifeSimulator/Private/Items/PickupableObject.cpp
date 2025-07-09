// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupableObject.h"
#include "Camera/CameraComponent.h"
#include "Characters/FPS_Character.h"

// Sets default values
APickupableObject::APickupableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	bIsPickedUp = true;
	AFPS_Character* player = Cast<AFPS_Character>(Picker);
	if (player) {
		FTransform WorldTransform = GetActorTransform();
		FTransform CameraTransform = player->GetCamera()->GetComponentTransform();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *WorldTransform.GetLocation().ToString());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *CameraTransform.GetLocation().ToString());

		FTransform RelativeTransform = WorldTransform.GetRelativeTransform(CameraTransform);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *RelativeTransform.GetLocation().ToString());

		AttachToComponent(player->GetCamera(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

		SetActorRelativeTransform(RelativeTransform);

		bShouldMove = true;
	}
}

// Called every frame
void APickupableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldMove) {
		FVector CurrentLocation = GetRootComponent()->GetRelativeLocation();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *TargetLocation.ToString());
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 5.0f);
		GetRootComponent()->SetRelativeLocation(NewLocation);

		FRotator NewRotation = FMath::RInterpTo(GetRootComponent()->GetRelativeRotation(), FRotator(0.0f, 0.0f, 0.0f), DeltaTime, 5.0f);
		GetRootComponent()->SetRelativeRotation(NewRotation);

		if (FVector::Dist(CurrentLocation, TargetLocation) < 1.f)
		{
			GetRootComponent()->SetRelativeLocation(TargetLocation);
			bShouldMove = false;
		}
	}
}

