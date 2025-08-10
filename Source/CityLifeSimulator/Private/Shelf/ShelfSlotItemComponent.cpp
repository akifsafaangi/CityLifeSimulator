// Fill out your copyright notice in the Description page of Project Settings.


#include "Shelf/ShelfSlotItemComponent.h"

// Sets default values for this component's properties
UShelfSlotItemComponent::UShelfSlotItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsTransferring = false;
    TransferDuration = 0.6f;
    ElapsedTime = 0.f;
    PendingDestination = nullptr;
	TempMesh = nullptr;
}


// Called when the game starts
void UShelfSlotItemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!VisualMesh)
    {
        TArray<USceneComponent*> Children;
        GetChildrenComponents(false, Children);
        for (USceneComponent* Child : Children)
        {
            if (UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(Child))
            {
                VisualMesh = SMC;
                break;
            }
        }
    }
}


// Called every frame
void UShelfSlotItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsTransferring || !TempMesh || !PendingDestination)
    {
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = TransferDuration <= 0.f ? 1.f : FMath::Clamp(ElapsedTime / TransferDuration, 0.f, 1.f);

    float SmoothAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.0f);

    FVector NewLoc = FMath::Lerp(StartLocationWS, TargetLocationWS, SmoothAlpha);
    FQuat NewQuat = FQuat::Slerp(StartRotationWS, TargetRotationWS, SmoothAlpha);
    FVector NewScale = FMath::Lerp(StartScaleWS, TargetScaleWS, SmoothAlpha);

    TempMesh->SetWorldLocationAndRotation(NewLoc, NewQuat);
    TempMesh->SetWorldScale3D(NewScale);

    if (Alpha >= 1.f - KINDA_SMALL_NUMBER)
    {
        FinishTransfer();
    }
}

// Start the transfer process
void UShelfSlotItemComponent::StartTransfer(UShelfSlotItemComponent* DestinationSlot, float Duration)
{
	if (!DestinationSlot || !VisualMesh || !StoredItem.Mesh)
    {
        return;
    }

    if (bIsTransferring)
    {
        CancelTransfer();
    }

    PendingDestination = DestinationSlot;
    TransferDuration = FMath::Max(0.01f, Duration);
    ElapsedTime = 0.f;
    bIsTransferring = true;

    TempMesh = NewObject<UStaticMeshComponent>(GetOwner());
    TempMesh->RegisterComponent();
    TempMesh->SetStaticMesh(StoredItem.Mesh);
    if (StoredItem.Material)
    {
        TempMesh->SetMaterial(0, StoredItem.Material);
    }
    TempMesh->SetWorldTransform(VisualMesh->GetComponentTransform());

    VisualMesh->SetVisibility(false);

    StartLocationWS = TempMesh->GetComponentLocation();
    StartRotationWS = TempMesh->GetComponentQuat();
    StartScaleWS = TempMesh->GetComponentScale();

    FTransform DestWorldTransform = PendingDestination->GetComponentTransform();
    TargetLocationWS = DestWorldTransform.GetLocation();
    TargetRotationWS = DestWorldTransform.GetRotation();
    TargetScaleWS = PendingDestination->GetComponentScale();
}

void UShelfSlotItemComponent::FinishTransfer()
{
    if (!bIsTransferring || !TempMesh || !PendingDestination)
    {
        bIsTransferring = false;
        PendingDestination = nullptr;
        return;
    }

    TempMesh->DestroyComponent();
    TempMesh = nullptr;

    PendingDestination->StoredItem = StoredItem;
    if (PendingDestination->VisualMesh)
    {
        PendingDestination->VisualMesh->SetStaticMesh(StoredItem.Mesh);
        if (StoredItem.Material)
        {
            PendingDestination->VisualMesh->SetMaterial(0, StoredItem.Material);
        }
        PendingDestination->VisualMesh->SetVisibility(true);
    }

    StoredItem = FItemData();
    VisualMesh->SetStaticMesh(nullptr);
    VisualMesh->SetVisibility(false);

    //OnTransferFinished.Broadcast(PendingDestination);

    bIsTransferring = false;
    ElapsedTime = 0.f;
    PendingDestination = nullptr;
}


void UShelfSlotItemComponent::CancelTransfer()
{
    if (!bIsTransferring)
        return;

    if (TempMesh)
    {
        TempMesh->DestroyComponent();
        TempMesh = nullptr;
    }

    bIsTransferring = false;
    ElapsedTime = 0.f;
    PendingDestination = nullptr;
}