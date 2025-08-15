// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Shelf/FItemData.h"
#include "ShelfSlotItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransferFinished, UShelfSlotItemComponent*, DestinationSlot);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CITYLIFESIMULATOR_API UShelfSlotItemComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShelfSlotItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shelf Slot Item")
	FItemData StoredItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shelf Slot Item")
	UStaticMeshComponent* VisualMesh;

	UFUNCTION(BlueprintCallable, Category="Item|Transfer")
	void StartTransfer(UShelfSlotItemComponent* DestinationSlot, float Duration);

	UFUNCTION(BlueprintCallable, Category="Item|Transfer")
	void CancelTransfer();

	UPROPERTY(BlueprintAssignable, Category="Item|Transfer")
    FOnTransferFinished OnTransferFinished;
	
	UPROPERTY()
	UStaticMeshComponent* TempMesh;

	UFUNCTION()
	void SetItemData(const FItemData& NewItemData);
	
	
	void ClearItemData();

	bool bIsMoving;
protected:
	bool bIsTransferring;
    float TransferDuration;
    float ElapsedTime;

    // Transfer transforms in world space
    FVector StartLocationWS;
    FQuat StartRotationWS;
    FVector StartScaleWS;

    FVector TargetLocationWS;
    FQuat TargetRotationWS;
    FVector TargetScaleWS;

    // Destination
    UPROPERTY()
    UShelfSlotItemComponent* PendingDestination;

    // Helper
    void FinishTransfer();
private:
	FVector CalculateObjectScale(UPrimitiveComponent* SourceComponent, USceneComponent* DestinationComponent);
};
