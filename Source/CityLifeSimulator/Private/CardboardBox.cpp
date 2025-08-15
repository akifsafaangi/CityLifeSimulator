// Fill out your copyright notice in the Description page of Project Settings.


#include "CardboardBox.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Shelf/ShelfSlotItemComponent.h"
#include "Components/BoxComponent.h"

ACardboardBox::ACardboardBox()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PickupCameraOffset = 200.f; // Default offset for the camera when picking up
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

void ACardboardBox::BeginPlay()
{
	Super::BeginPlay();
}

void ACardboardBox::Tick(float DeltaTime)
{
    if (CurrentInteractor && PhysicsHandle && PhysicsHandle->GrabbedComponent)
    {
        AFPS_Character* Player = Cast<AFPS_Character>(CurrentInteractor);
        if (Player && Player->GetCamera())
        {
            FVector TargetLoc = Player->GetCamera()->GetComponentLocation() + Player->GetCamera()->GetForwardVector() * PickupCameraOffset;
            TargetLoc.Z -= 80.f; // Adjust height if necessary
            FRotator TargetRot = Player->GetCamera()->GetComponentRotation();
            PhysicsHandle->SetTargetLocationAndRotation(TargetLoc, TargetRot);
        }
    }
}

void ACardboardBox::Pickup(AActor* Picker)
{
    UPrimitiveComponent* RootPrim = FindComponentByClass<UPrimitiveComponent>();

	if (!RootPrim) return;

	CurrentInteractor = Picker;
	bIsPickedUp = true;
	AFPS_Character* Player = Cast<AFPS_Character>(Picker);
	if (Player && Player->GetCamera())
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(RootPrim, NAME_None, RootPrim->GetComponentLocation(), RootPrim->GetComponentRotation());
	}
}

void ACardboardBox::Interact_Implementation(AActor* Interactor)
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

void ACardboardBox::Throw()
{
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
	}
	SetCurrentInteractor(nullptr);
}

void ACardboardBox::OpenBox(AActor* Interactor)
{
	OpenCloseBox();
}

void ACardboardBox::MoveObject(UShelfSlotItemComponent* TargetSlot, float Duration, bool bDirection)
{
	if (!TargetSlot) return;

	for (UShelfSlotItemComponent* Item : Items)
	{
		if (Item && !Item->bIsMoving)
		{
			if (bDirection) {
				if (Item->VisualMesh->GetStaticMesh())
				{
					TargetSlot->bIsMoving = true;
					Item->StartTransfer(TargetSlot, Duration);
					return;
				}
			} else {
				if (!Item->VisualMesh->GetStaticMesh())
				{
					UE_LOG(LogTemp, Warning, TEXT("Moving item: %s"), *Item->GetName());
					Item->bIsMoving = true;
					TargetSlot->StartTransfer(Item, Duration);
					return;
				}
			}
		}
	}
}

void ACardboardBox::Init(UStaticMesh* NewItemMesh, UMaterialInterface* NewDefaultItemMaterial, int size)
{
	if (NewItemMesh)
	{
		for (int i = 0; i < size; i++)
		{
			UShelfSlotItemComponent* NewItem = NewObject<UShelfSlotItemComponent>(this);
			if (NewItem)
			{
				NewItem->RegisterComponent();
				NewItem->AttachToComponent(BoxCollision, FAttachmentTransformRules::KeepRelativeTransform);
				 if (NewItem->VisualMesh)
				{
					NewItem->VisualMesh->RegisterComponent();
					NewItem->VisualMesh->AttachToComponent(NewItem, FAttachmentTransformRules::KeepRelativeTransform);
				}
				Items.Add(NewItem);
				FItemData ItemData;
				ItemData.ItemID = FName(*FString::Printf(TEXT("Item_%d"), Items.IndexOfByKey(NewItem)));
				ItemData.Mesh = NewItemMesh;
				ItemData.Material = NewDefaultItemMaterial;

				NewItem->SetItemData(ItemData);
				UE_LOG(LogTemp, Warning, TEXT("Initialized item: %s"), *ItemData.ItemID.ToString());
			}
		}
	}
}